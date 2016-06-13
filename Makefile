.PHONY: build test
.DEFAULT_GOAL := test

CXX ?= clang++
CXXFLAGS = -std=c++11 -Wall -ggdb3 -fsanitize=undefined -DDEBUG -D_GLIBCXX_DEBUG

build: a.out
a.out: a.cpp
	${CXX} ${CXXFLAGS} $<
online-exam/tools/interactor:
	${CXX} ${CXXFLAGS} -o $@ $@.cpp
online-exam/solutions/solution: a.out
	cp $< $@
test: online-exam/solutions/solution online-exam/tools/interactor
	( cd online-exam ; sh test_solution.sh ) 2>&1 | tee /dev/tty | grep 'ok best = \w\+, score = \w\+' | awk '{ sum += $$7 } END { print "done average score =", sum / NR }'
