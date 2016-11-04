Abstracted the buffer size and header in a single file prodcon.h

1.	Run the command 'make clean'. This will delete the following artifacts:
	a. 	Object file for main (main.o)
	b.	Object file for producer (prod.o)
	c.	Object file for consumer (cons.o)
	d.	Text file for producers (producer_WHITE.txt, producer_BLACK.txt, producer_RED.txt)
	e.	Text file for consumer (consumer.txt)

2.	Run the command 'make'. This will create three object file
	a. 	main.o
	b.	prod.o
	c.	cons.o

3.	Run the command './main'. This will create three producer text files and a consumer file.
	Each producer file will have 1000 entries and consumer file will have 3000 entries
