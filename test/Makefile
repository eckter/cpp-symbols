CXXFLAGS=-std=c++14 -g -Wall -Wextra -pedantic -fPIC
LDFLAGS=-ldl
TARGETS=test libtest.so

all: test libtest.so

libtest.so: libtest.o
	$(CC) -shared -o $@ $^ -fPIC

# the implicit rules places test.cc before LDFLAGS,
# which results in failing to link with dlsym/dladdr
test: test.cc
	$(CXX) $^ $(CXXFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f $(TARGETS) *.o
