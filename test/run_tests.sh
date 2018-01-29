#! /bin/sh

cd "$(dirname "$0")"

echo "Compiling..."
make > /dev/null

echo "Running..."
./test "./libtest.so"

if [ $? -eq 0 ]; then
    echo "Success"
else
    echo "Failure"
fi

make clean > /dev/null
