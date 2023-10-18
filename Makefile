menu: menu.c index_first_even.c index_last_odd.c sum_between_even_odd.c sum_before_even_and_after_odd.c
gcc -Wall -std=c99 -o $@ $^

clean:
rm -f menu
