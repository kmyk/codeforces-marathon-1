.PHONY: build test
.DEFAULT_GOAL := test

CXX ?= clang++
CXXFLAGS = -std=c++14 -Wall -ggdb3 -fsanitize=undefined -DDEBUG -D_GLIBCXX_DEBUG
build: a.out
a.out: a.cpp
	${CXX} ${CXXFLAGS} $<
online-exam/tools/interactor:
	${CXX} ${CXXFLAGS} -o $@ $@.cpp
online-exam/solutions/solution: a.out
	cp $< $@
test: online-exam/solutions/solution online-exam/tools/interactor
	( cd online-exam ; sh test_solution.sh )
