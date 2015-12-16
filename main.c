/* 
 * File:   main.c
 * Author: alnaser
 *
 * Created on 1 décembre 2015, 19:58
 */

#include <stdio.h>
#include <string.h>
#include <ncurses.h>// il faut installé cette bibliotheque 
// http://www.cyberciti.biz/faq/linux-install-ncurses-library-headers-on-debian-ubuntu-centos-fedora/
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


typedef char String[30];

void parseVarargs(char *line, char **argv) {
    while (*line != '\0') { 
        while (*line == ' ' || *line == '\t' || *line == '\n')
            *line++ = '\0'; 
        *argv++ = line; 
        while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
            line++; 
    }
    *argv = '\0'; 
}


void authentifcation(WINDOW * win) {
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    int i = 0;
    String password;
    char c;
    wmove(win, 4, COLS / 2 - 10);
    wprintw(win, "Authentification: ");
    noecho(); //disactivé laffichage
    while ((c = wgetch(win)) != '\n') {
        password[i] = c;
        i++;
        wprintw(win, "*");
    }
    password[i] = '\0';
    if (strcmp(password, "123")) {
        wprintw(win, "  mot de passe incorrecte!!!");
        wgetch(win);
        wclear(win);
        wrefresh(win);
        authentifcation(win);
    }
    echo(); //activé laffichage

}


void execute(char **argv, WINDOW *win) {
    pid_t pid;
    pid = fork();

    switch (pid) {
        case -1: wprintw(win, "*** ERROR: forking child process failed\n");
            break;
        case 0:
//            wrefresh(win);
            endwin();
            nl();
            if (execvp(argv[0], argv) < 0) {
                wprintw(win, "*** ERROR: exec failed\n");
//                wbkgd(win,COLOR_RED);
                wgetch(win);
                exit(EXIT_FAILURE);
            }
            
            break;
        default:
            wait(NULL);
            wgetch(win);
            wclear(win);
            wrefresh(win);
            break;
    }
}

void drawMenuHeader(WINDOW *win, WINDOW * winBody) {

    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    wborder(winBody, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(winBody);
    wrefresh(win);
    char *argv[20];
    String lincmd;
    mvwprintw(win, 2, COLS / 2 - 10, "Bienvenue sur Mini Shell ");
    mvwprintw(win, 8, 5, "ecrire vos comande >");
    wgetstr(win, lincmd);
    while (strcmp("quit", lincmd) == 0 || strcmp("exit", lincmd) == 0){
        endwin();
        exit(EXIT_SUCCESS);
    }
        

    parseVarargs(lincmd, argv);
    wmove(winBody, 3, 3);
    wrefresh(winBody);
    execute(argv, winBody);
    wclear(win);
    wrefresh(win);
    drawMenuHeader(win, winBody);
}

int main(int argc, char** argv) {
    int contr;
    char cmd[50], separator = '\0', ch = ' ';
    char *commandLine[50];
    char *commandResult[50];
    WINDOW *winHeader;
    WINDOW *winBody;
    WINDOW *winAuth;
    initscr();
//    nonl();
    if ((winAuth = newwin(10, COLS, 10, 0)) == NULL) {
        addstr("cannot open window");
        endwin();
        return 1;
    }
    authentifcation(winAuth);

    if ((winHeader = newwin(10, COLS, LINES-10, 0)) == NULL) {
        addstr("cannot open window");
        endwin();
        return 1;
    }


    if ((winBody = newwin(LINES - 11, COLS, 0, 0)) == NULL) {
        addstr("cannot open window");
        endwin();
        return 1;
    }
    drawMenuHeader(winHeader, winBody);
    return (EXIT_SUCCESS);
}
