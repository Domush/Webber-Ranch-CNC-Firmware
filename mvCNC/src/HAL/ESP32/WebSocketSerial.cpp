/**
 * Modern Vintage CNC Firmware
*/
#ifdef ARDUINO_ARCH_ESP32

#include "../../inc/mvCNCConfigPre.h"

#if ENABLED(WIFISUPPORT)

#include "WebSocketSerial.h"
#include "wifi.h"
#include <ESPAsyncWebServer.h>

MSerialWebSocketT webSocketSerial(false);
AsyncWebSocket ws("/ws"); // TODO Move inside the class.

// RingBuffer impl

#define NEXT_INDEX(I, SIZE) ((I + 1) & (ring_buffer_pos_t)(SIZE - 1))

RingBuffer::RingBuffer(ring_buffer_pos_t size)
  : data(new uint8_t[size]),
    size(size),
    read_index(0),
    write_index(0)
{}

RingBuffer::~RingBuffer() { delete[] data; }

ring_buffer_pos_t RingBuffer::write(const uint8_t c) {
  const ring_buffer_pos_t n = NEXT_INDEX(write_index, size);

  if (n != read_index) {
    this->data[write_index] = c;
    write_index = n;
    return 1;
  }

  // TODO: buffer is full, handle?
  return 0;
}

ring_buffer_pos_t RingBuffer::write(const uint8_t *buffer, ring_buffer_pos_t size) {
  ring_buffer_pos_t written = 0;
  for (ring_buffer_pos_t i = 0; i < size; i++) {
    written += write(buffer[i]);
  }
  return written;
}

int RingBuffer::available() {
  return (size - read_index + write_index) & (size - 1);
}

int RingBuffer::peek() {
  return available() ? data[read_index] : -1;
}

int RingBuffer::read() {
  if (available()) {
    const int ret = data[read_index];
    read_index = NEXT_INDEX(read_index, size);
    return ret;
  }
  return -1;
}

ring_buffer_pos_t RingBuffer::read(uint8_t *buffer) {
   ring_buffer_pos_t len = available();

  for (ring_buffer_pos_t i = 0; read_index != write_index; i++) {
    buffer[i] = data[read_index];
    read_index = NEXT_INDEX(read_index, size);
  }

  return len;
}

void RingBuffer::flush() { read_index = write_index; }

// WebSocketSerial impl
WebSocketSerial::WebSocketSerial()
    : rx_buffer(RingBuffer(RX_BUFFER_SIZE)),
      tx_buffer(RingBuffer(TX_BUFFER_SIZE))
{}

void WebSocketSerial::begin(const long baud_setting) {
  ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type) {
      case WS_EVT_CONNECT: client->ping(); break; // client connected
      case WS_EVT_DISCONNECT:                     // client disconnected
      case WS_EVT_ERROR:                          // error was received from the other end
      case WS_EVT_PONG: break;                    // pong message was received (in response to a ping request maybe)
      case WS_EVT_DATA: {                         // data packet
        AwsFrameInfo * info = (AwsFrameInfo*)arg;
        if (info->opcode == WS_TEXT || info->message_opcode == WS_TEXT)
          this->rx_buffer.write(data, len);
      }
    }
  });
  server.addHandler(&ws);
}

void WebSocketSerial::end() { }
int WebSocketSerial::peek() { return rx_buffer.peek(); }
int WebSocketSerial::read() { return rx_buffer.read(); }
int WebSocketSerial::available() { return rx_buffer.available(); }
void WebSocketSerial::flush() { rx_buffer.flush(); }

size_t WebSocketSerial::write(const uint8_t c) {
  size_t ret = tx_buffer.write(c);

  if (ret && c == '\n') {
    uint8_t tmp[TX_BUFFER_SIZE];
    ring_buffer_pos_t size = tx_buffer.read(tmp);
    ws.textAll(tmp, size);
  }

  return ret;
}

size_t WebSocketSerial::write(const uint8_t *buffer, size_t size) {
  size_t written = 0;
  for (size_t i = 0; i < size; i++)
    written += write(buffer[i]);
  return written;
}

#endif // WIFISUPPORT
#endif // ARDUINO_ARCH_ESP32
