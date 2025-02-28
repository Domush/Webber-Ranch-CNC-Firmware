help:
	@echo "Tasks for local development:"
	@echo "* tests-single-ci:             Run a single test from inside the CI"
	@echo "* tests-single-local:          Run a single test locally"
	@echo "* tests-single-local-docker:   Run a single test locally, using docker-compose"
	@echo "* tests-all-local:             Run all tests locally"
	@echo "* tests-all-local-docker:      Run all tests locally, using docker-compose"
	@echo "* setup-local-docker:          Setup local docker-compose"
	@echo ""
	@echo "Options for testing:"
	@echo "  TEST_TARGET          Set when running tests-single-*, to select the"
	@echo "                       test. If you set it to ALL it will run all "
	@echo "                       tests, but some of them are broken: use "
	@echo "                       tests-all-* instead to run only the ones that "
	@echo "                       run on GitHub CI"
	@echo "  ONLY_TEST            Limit tests to only those that contain this, or"
	@echo "                       the index of the test (1-based)"
	@echo "  VERBOSE_PLATFORMIO   If you want the full PIO output, set any value"
	@echo "  GIT_RESET_HARD       Used by CI: reset all local changes. WARNING:"
	@echo "                       THIS WILL UNDO ANY CHANGES YOU'VE MADE!"
.PHONY: help

tests-single-ci:
	export GIT_RESET_HARD=true
	$(MAKE) tests-single-local TEST_TARGET=$(TEST_TARGET)
.PHONY: tests-single-ci

tests-single-local:
	@if ! test -n "$(TEST_TARGET)" ; then echo "***ERROR*** Set TEST_TARGET=<your-module> or use make tests-all-local" ; return 1; fi
	export PATH=./buildroot/bin/:./buildroot/tests/:${PATH} \
	  && export VERBOSE_PLATFORMIO=$(VERBOSE_PLATFORMIO) \
	  && run_tests . $(TEST_TARGET) "$(ONLY_TEST)"
.PHONY: tests-single-local

tests-single-local-docker:
	@if ! test -n "$(TEST_TARGET)" ; then echo "***ERROR*** Set TEST_TARGET=<your-module> or use make tests-all-local-docker" ; return 1; fi
	docker-compose run --rm mvcnc $(MAKE) tests-single-local TEST_TARGET=$(TEST_TARGET) VERBOSE_PLATFORMIO=$(VERBOSE_PLATFORMIO) GIT_RESET_HARD=$(GIT_RESET_HARD) ONLY_TEST="$(ONLY_TEST)"
.PHONY: tests-single-local-docker

tests-all-local:
	export PATH=./buildroot/bin/:./buildroot/tests/:${PATH} \
	  && export VERBOSE_PLATFORMIO=$(VERBOSE_PLATFORMIO) \
	  && for TEST_TARGET in $$(./get_test_targets.py) ; do echo "Running tests for $$TEST_TARGET" ; run_tests . $$TEST_TARGET ; done
.PHONY: tests-all-local

tests-all-local-docker:
	docker-compose run --rm mvcnc $(MAKE) tests-all-local VERBOSE_PLATFORMIO=$(VERBOSE_PLATFORMIO) GIT_RESET_HARD=$(GIT_RESET_HARD)
.PHONY: tests-all-local-docker

setup-local-docker:
	docker-compose build
.PHONY: setup-local-docker
