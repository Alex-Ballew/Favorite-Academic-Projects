giza: giza.s
	gcc -g -no-pie giza.s -o giza

wc: wc.s
	gcc -g -no-pie wc.s -o wc

clean:
	rm -f *.o giza


test-giza-1: giza
	./giza 1

test-giza-2: giza
	./giza 2

test-giza-5: giza
	./giza 5

test-giza-10: giza
	./giza 10

test-giza-21: giza
	./giza 21

test-giza: test-giza-1 test-giza-2 test-giza-5 test-giza-10 test-giza-21

test-wc-empty: wc
	echo "" | ./wc

test-wc-one: wc
	echo "single" | ./wc

test-wc-space: wc
	echo "globins and ghouls" | ./wc

test-wc-multi: wc
	echo -e "\tTo be   or not\n  to be \r" | ./wc

test-wc: test-wc-empty test-wc-one test-wc-space test-wc-multi
