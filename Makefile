CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic

STRUCTURES = src/array/array.cpp \
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
	python3 tools/run_tests.py google

build/boost_tests: $(BOOST_TESTS) $(STRUCTURES) $(HEADERS) $(BOOST)/boost/version.hpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -Isrc -isystem $(BOOST) $(BOOST_TESTS) $(STRUCTURES) -pthread -o $@

test-boost: build/boost_tests
	python3 tools/run_tests.py boost

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
coverage-cpp: $(COVER_BUILD)/google $(COVER_BUILD)/boost $(COVER_BUILD)/app
	python3 tools/coverage_cpp.py

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
	python3 tools/check_go_format.py
	cd go && $(GO_ENV) $(GO) vet ./...

coverage-go:
	mkdir -p reports/go
	cd go && $(GO_ENV) $(GO) test -count=1 -covermode=atomic -coverprofile=../reports/go/coverage.out ./...
	cd go && $(GO_ENV) $(GO) tool cover -html=../reports/go/coverage.out -o ../reports/go/index.html
	cd go && $(GO_ENV) $(GO) tool cover -func=../reports/go/coverage.out > ../reports/go/summary.txt
	cat reports/go/summary.txt
	python3 tools/coverage_go_check.py

coverage: coverage-cpp coverage-go
	python3 tools/coverage_index.py
