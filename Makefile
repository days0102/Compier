-include */*.d

BFLAGS = -d

# CXX=g++ #使用LLVM时会出错
CXX=clang++
CXXFLAGES=-g -MD
CXXFLAGES+= -Wno-write-strings
CXXFLAGES+= -Wno-deprecated-register # 禁止c++17 register 警告
CXXFLAGES+= -std=c++11
CXXFLAGES+= -I. -Iinc
CXXFLAGES+= -Illvm
CXXFLAGES+= -Illvm-c
# CXXFLAGES+= -I/usr/include/llvm-10
# CXXFLAGES+= -I/usr/include/llvm-c-10
# LLVM 参数
CXXFLAGES+= `llvm-config --cxxflags --ldflags --system-libs --libs core` #--link-static
CXXFLAGES+= -DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING

SRCS =  src/main.cc \
		src/tokens.cc \
		src/tree.cc \
		src/symbols.cc \
		src/codegen.cc \
		bison.tab.cc \
	
OBJS = $(SRCS:.cc=.o)
OBJS += $(SRCS:.c=.o)

TARGET= parser

all: $(SRCS)
	${CXX} ${CXXFLAGES} ${SRCS} -lfl -o ${TARGET}

lex.yy.c: flex.l
	flex flex.l

bison.tab.cc: lex.yy.c bison.y
	bison -d bison.y
	mv bison.tab.c bison.tab.cc
	mv bison.tab.h bison.tab.hh

.PRECIOUS: %.o # 保留中间过程的 .o 文件
# %.o : %.cc
# 	${CXX} ${CXXFLAGES} -c -o $@ $<
# %.o : %.c
# 	${CXX} ${CXXFLAGES} -c -o $@ $<

# bison.tab.c: bison.y
# 	bison ${BFLAGS} bison.y

# flex: flex.l
# 	flex flex.l

# bison: bison.y
# 	bison ${BFLAGS} bison.y

clean:
	rm *.tab.* lex.yy.c parser 2> /dev/null || :

run:${TARGET} ${SRCS}
	@./parser text.dd