prod_con	:	main	prod	cons
main	:	main.c prodcon.h
	gcc	-o	main	main.c -lpthread
prod	:	producer.c prodcon.h
	gcc	-o	prod	producer.c -lpthread
cons	:	consumer.c prodcon.h
	gcc	-o	cons	consumer.c -lpthread
clean	:
	rm main	prod cons Consumer.txt Producer_WHITE.txt Producer_BLACK.txt Producer_RED.txt	
