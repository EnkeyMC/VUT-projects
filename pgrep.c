#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <queue>
#include <mutex>
#include <vector>
#include <iostream>
#include <string.h>
#include <regex>

#define debugs(s) fprintf(stderr, "%d: %s\n", __LINE__, s); fflush(stderr);
#define debug(format, ...) fprintf(stderr, "%d: " format "\n", __LINE__, __VA_ARGS__); fflush(stderr);


std::vector<std::mutex *> read_locks; /* pole zamku promenne velikosti */

char *line;
long int line_score;
int read_cnt;
std::mutex *line_score_lock;
std::mutex *read_cnt_lock;
std::mutex *main_lock;

char *read_line(int *res) {
    std::string line;
    char *str;
    if (std::getline(std::cin, line)) {
        str=(char *) malloc(sizeof(char)*(line.length()+1));
        strcpy(str,line.c_str());
        *res=1;
        return str;
    } else {
        *res=0;
        return NULL;
    }
}


void re_pattern_check(int ID, char* pattern, long int score) {
	debug("Thread %i started",ID);
    std::regex re (pattern);

    do {
        debug("%d - Waiting for line", ID);
        read_locks[ID]->lock();  // Wait for main to give control
        debug("%d - Matching regex", ID);
        if (std::regex_match(line, re)) {
            debug("%d - Line matches %s", ID, pattern);
            line_score_lock->lock();
            line_score += score;
            line_score_lock->unlock();
        }

        read_cnt_lock->lock();
        read_cnt--;
        if (read_cnt == 0) {
            main_lock->unlock(); // Give main control back
        }
        read_cnt_lock->unlock();
    } while(true);
}

int main(int argv, char* args[]) {
    if (argv < 4)
        return EXIT_FAILURE;

    if (argv % 2 != 0)
        return EXIT_FAILURE;

    long int min_score = strtol(args[1], NULL, 10);

    int re_num = (argv - 2) / 2; // Number of regular expresions

    char* re_patterns[re_num];
    long int re_scores[re_num];
    int i;

    for (i = 2; i < argv; i += 2) {
        re_patterns[(i - 2) / 2] = args[i];
        re_scores[(i - 2) / 2] = strtol(args[i + 1], NULL, 10);
    }

    /*******************************
     * Inicializace threadu a zamku
     * *****************************/

    /* vytvorime zamky */
    read_locks.resize(re_num); /* nastavime si velikost pole zamky */
    for(int i=0;i<re_num;i++){
        std::mutex *new_lock = new std::mutex();
        read_locks[i] = new_lock;
        new_lock->lock();
    }

    line_score_lock = new std::mutex();
    read_cnt_lock = new std::mutex();
    main_lock = new std::mutex();

    std::vector <std::thread *> threads; /* pole threadu promenne velikosti */
    /* vytvorime thready */
	threads.resize(re_num); /* nastavime si velikost pole threads */
	for(int i=0;i<re_num;i++){
		std::thread *new_thread = new std::thread (re_pattern_check, i, re_patterns[i], re_scores[i]);
		threads[i] = new_thread;
	}
	/**********************************
	 * Vlastni vypocet pgrep
	 * ********************************/
	int res;
	line=read_line(&res);

	while (res) {
        read_cnt = re_num;
        for (int i = 0; i < re_num; i++) {
            read_locks[i]->unlock();
        }
        main_lock->lock();

        if (line_score >= min_score)
		    printf("%s\n",line);
		free(line); /* uvolnim pamet */
		line=read_line(&res);
	}

    for (int i = 0; i < re_num; i++) {
        read_locks[i]->unlock();
    }


	/**********************************
	 * Uvolneni pameti
	 * ********************************/

	/* provedeme join a uvolnime pamet threads */
	for(int i=0;i<re_num;i++){
		threads[i]->join();
		delete threads[i];
	}
	/* uvolnime pamet zamku */
	for(int i=0;i<re_num;i++){
		delete read_locks[i];
	}

    delete read_cnt_lock;
    delete main_lock;
    delete line_score_lock;

	printf("everything finished\n");
	
}
