#include <stdio.h>
#include <string.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#define ERROR 1
#define WARN 2
#define INFO 3


char qwerty[3][15][1] = {{ "q", "w", "e", "r", "t", "y", "u", "i", "o", "p"}, { "a", "s", "d", "f", "g", "h", "j", "k", "l"}, { "z", "x", "c", "v", "b", "n", "m"}};

void jlog(int LOGLEVEL, char *message){

  char *color;
  char *levelmsg;

  switch(LOGLEVEL){
    case ERROR:
      color = BOLDRED;
      levelmsg = "ERR: ";
      break;
    case WARN:
      color = YELLOW;
      levelmsg = "WARN: ";
      break;
    case INFO:
      color = GREEN;
      levelmsg = "INFO: ";
      break;
    default:
      color = RESET;
      levelmsg = "";
      break;
  }
    
  printf("%s", color);
  printf("%s" RESET, levelmsg);
  printf(message,"\n");

}

void usage(char **argv) {

  printf("\nUsage: %s <known word> <unknown word>\n", argv[0]);

}

/* s, t: two strings; ls, lt: their respective length */
int levenshtein(char *s, int ls, char *t, int lt)
{
        int a, b, c;
        /* if either string is empty, difference is inserting all chars 
         * from the other
         */
        if (!ls) return lt;
        if (!lt) return ls;
 
        /* if first letters are the same, the difference is whatever is
         * required to edit the rest of the strings
         */
        if (s[ls] == t[lt])
                return levenshtein(s, ls - 1, t, lt - 1);
 
        /* else try:
         *      changing first letter of s to that of t; or
         *      remove first letter of s; or
         *      remove first letter of t,
         * any of which is 1 edit plus editing the rest of the strings
         */
        a = levenshtein(s, ls - 1, t, lt - 1);
        b = levenshtein(s, ls,     t, lt - 1);
        c = levenshtein(s, ls - 1, t, lt    );
 
        if (a > b) a = b;
        if (a > c) a = c;
 
        return a + 1;
}

int main(int argc, char *argv[])
{
        if(argc < 3) { 
          jlog(ERROR, "Not enough arguments");
          usage(argv);
          return 1;
        }

        char *s1 = argv[1];
        char *s2 = argv[2];
        jlog(INFO, "Parsing...");
        printf("distance between `%s' and `%s': %d\n", s1, s2,
                levenshtein(s1, strlen(s1), s2, strlen(s2)));

        int i, j, k;
        int letterRange = 1;
       
        int matchA[2], matchB[2];


        for(k=0; s1[k] != '\0'; k++){
          //letters are different
          if( s1[k] != s2[k]){
            matchA[0] = -1;
            matchA[1] = -1;
            matchB[0] = -1;
            matchB[1] = -1;

            //lookup the letters in the table
            for(i=0; i<3; i++){
              for(j=0; j<14; j++){
                if(qwerty[i][j][0] == s1[k]){
                  matchA[0] = i;
                  matchA[1] = j;
                }

                if(qwerty[i][j][0] == s2[k]){
                  matchB[0] = i;
                  matchB[1] = j;
                }
              
            }}
 

            if(matchA[0] > -1 && matchB[0] > -1){

              if(matchA[0] == matchB[0] || (matchA[0] - matchB[0] == 1) || (matchB[0] - matchA[0] == 1)){

                if( ((matchA[1] - matchB[1]) < 2) && ((matchA[1] - matchB[1]) > -1) ){
                  printf("suggestion: swap %c for %c\n", s1[k], s2[k]);
                }else if( ((matchB[1] - matchA[1]) < 2) && ((matchB[1] - matchA[1]) > -1) ){
                  printf("suggestion: swap %c for %c\n", s1[k], s2[k]);
                }
              }
                
            }
          }
        }



 
        return 0;
}

