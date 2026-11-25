CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic

STRUCTURES = src/storage/storage.cpp src/array/array.cpp \
             src/forward_list/forward_list.cpp \
             src/list/list.cpp \
             src/stack/stack.cpp \
             src/hash/hash_table.cpp \
             src/tree/tree.cpp
SOURCES = src/main.cpp $(wildcard src/cli/*.cpp) $(STRUCTURES)
GTEST = third_party/googletest
BOOST = third_party/boost
BOOST_TESTS = $(wildcard tests/boost/*.cpp)
TESTS = $(wildcard tests/test_*.cpp)
HEADERS = $(wildcard src/*.h src/*/*.h src/*/*.tpp)

.PHONY: all run test test-boost clean

all: build/lr3

build/lr3: $(SOURCES) $(HEADERS)
	mkdir -p build
	$(CXX) $(CXXFLAGS) -Isrc $(SOURCES) -o $@

run: build/lr3
	./build/lr3

build/unit_tests: $(TESTS) $(STRUCTURES) $(HEADERS) $(wildcard $(GTEST)/include/gtest/*.h $(GTEST)/include/gtest/internal/*.h $(GTEST)/src/*)
	mkdir -p build
	$(CXX) $(CXXFLAGS) -Isrc -isystem $(GTEST)/include -I$(GTEST) $(TESTS) $(STRUCTURES) $(GTEST)/src/gtest-all.cc $(GTEST)/src/gtest_main.cc -pthread -o $@

test: build/unit_tests
	./build/unit_tests

build/boost_tests: $(BOOST_TESTS) $(STRUCTURES) $(HEADERS) $(BOOST)/boost/version.hpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -Isrc -isystem $(BOOST) $(BOOST_TESTS) $(STRUCTURES) -pthread -o $@

test-boost: build/boost_tests
	./build/boost_tests

clean:
	rm -rf build

# Покрытие C++ включает структуры, меню и main; сторонний код исключён из отчёта.
COVER_FLAGS = $(CXXFLAGS) -O0 -g -fprofile-instr-generate -fcoverage-mapping
COVER_BUILD = build/coverage

$(COVER_BUILD):
	mkdir -p $@

$(COVER_BUILD)/google: $(TESTS) $(STRUCTURES) $(HEADERS) | $(COVER_BUILD)
	$(CXX) $(COVER_FLAGS) -Isrc -isystem $(GTEST)/include -I$(GTEST) $(TESTS) $(STRUCTURES) $(GTEST)/src/gtest-all.cc $(GTEST)/src/gtest_main.cc -pthread -o $@

$(COVER_BUILD)/boost: $(BOOST_TESTS) $(STRUCTURES) $(HEADERS) | $(COVER_BUILD)
	$(CXX) $(COVER_FLAGS) -Isrc -isystem $(BOOST) $(BOOST_TESTS) $(STRUCTURES) -pthread -o $@

$(COVER_BUILD)/app: $(SOURCES) $(HEADERS) | $(COVER_BUILD)
	$(CXX) $(COVER_FLAGS) -Isrc $(SOURCES) -o $@

.PHONY: coverage-cpp go run-go test-go coverage-go coverage lint-go
coverage-cpp: $(COVER_BUILD)/google $(COVER_BUILD)/boost $(COVER_BUILD)/catch $(COVER_BUILD)/app
	bash tools/coverage_cpp.sh

GO ?= go
GO_ENV = GOPATH=$(CURDIR)/build/go-path GOMODCACHE=$(CURDIR)/build/go-mod GOCACHE=$(CURDIR)/build/go-cache
GO_FILES = $(shell find go -name '*.go') go/go.mod go/go.sum

go: build/lr3-go

build/lr3-go: $(GO_FILES)
	mkdir -p build
	cd go && $(GO_ENV) $(GO) build -o ../build/lr3-go ./cmd/lr3

run-go: go
	./build/lr3-go

test-go:
	cd go && $(GO_ENV) $(GO) test -count=1 ./...

lint-go:
	bash tools/check_go_format.sh
	cd go && $(GO_ENV) $(GO) vet ./...

coverage-go:
	mkdir -p reports/go
	cd go && $(GO_ENV) $(GO) test -count=1 -covermode=atomic -coverprofile=../reports/go/coverage.out ./...
	cd go && $(GO_ENV) $(GO) tool cover -html=../reports/go/coverage.out -o ../reports/go/index.html
	cd go && $(GO_ENV) $(GO) tool cover -func=../reports/go/coverage.out > ../reports/go/summary.txt
	cat reports/go/summary.txt
	bash tools/coverage_go_check.sh

coverage: coverage-cpp coverage-go
	bash tools/coverage_index.sh

.PHONY: test-serialization
test-serialization: all go
	bash tests/test_serialization.sh

CATCH = third_party/catch2
CATCH_TESTS = $(wildcard tests/catch/*.cpp)
.PHONY: test-catch benchmark benchmark-cpp benchmark-go
build/catch_tests: $(CATCH_TESTS) $(STRUCTURES) $(HEADERS) $(CATCH)/catch_amalgamated.cpp $(CATCH)/catch_amalgamated.hpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -Isrc -isystem $(CATCH) $(CATCH_TESTS) $(STRUCTURES) $(CATCH)/catch_amalgamated.cpp -pthread -o $@

test-catch: build/catch_tests
	./build/catch_tests

build/benchmark: benchmarks/benchmark.cpp $(STRUCTURES) $(HEADERS)
	mkdir -p build
	$(CXX) $(CXXFLAGS) -O2 -Isrc benchmarks/benchmark.cpp $(STRUCTURES) -o $@

benchmark-cpp: build/benchmark
	mkdir -p reports/benchmark
	./build/benchmark > reports/benchmark/cpp.txt
	cat reports/benchmark/cpp.txt

benchmark-go:
	mkdir -p reports/benchmark
	cd go && $(GO_ENV) $(GO) test ./structures -run '^$$' -bench . -benchmem -count=1 > ../reports/benchmark/go.txt
	cat reports/benchmark/go.txt

benchmark: benchmark-cpp benchmark-go

$(COVER_BUILD)/catch: $(CATCH_TESTS) $(STRUCTURES) $(HEADERS) | $(COVER_BUILD)
	$(CXX) $(COVER_FLAGS) -Isrc -isystem $(CATCH) $(CATCH_TESTS) $(STRUCTURES) $(CATCH)/catch_amalgamated.cpp -pthread -o $@

CPPCHECK ?= cppcheck
.PHONY: lint-cpp
lint-cpp:
	xcrun clang-format --dry-run --Werror $(wildcard src/*.cpp src/*/*.cpp src/*/*.h src/*/*.tpp tests/test_*.cpp tests/boost/*.cpp tests/catch/*.cpp benchmarks/*.cpp)
	@mkdir -p reports/lint
	@$(CPPCHECK) --version
	@$(CPPCHECK) --std=c++17 --language=c++ --enable=warning,performance,portability --error-exitcode=1 --inline-suppr -Isrc src benchmarks > reports/lint/cppcheck.txt 2>&1 || { cat reports/lint/cppcheck.txt; exit 1; }
	@cat reports/lint/cppcheck.txt
	@echo "Cppcheck: анализ C++ завершён, замечаний не найдено."
