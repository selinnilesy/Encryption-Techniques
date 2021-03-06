#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* IT HAS BEEN GIVEN THAT STATIC ARRAYS ARE FORBIDDEN AND POINTERS MUST BE USED FOR THIS EXAM.*/

int get_string_length(char *string) {
    int i = 0;
    if (string != NULL) {
        while (string[i] != '\0') i++;
    }
    return i;
}
int get_longest_string_length(char **s, int n_string) {
    int maxLength = 0, tempLength = 0;
    int i;

    if (s == NULL) {
        return 0;
    }
    for (i = 0; i < n_string; i++) {
        if (s[i] != NULL) {
            tempLength = get_string_length(s[i]);
            if (tempLength > maxLength) {
                maxLength = tempLength;
            }
        }
    }
    return maxLength;
}

void print_combined_strings(char **s, int n_string) {
    int i;
    for (i = 0; i < n_string; i++) {
        if (s[i] != NULL) {
            printf("%s\n", s[i]);
        }
    }
}
int is_letter(char c) {
    return (c < 91 && c > 64) || (c < 123 && c > 96);
}
/*
 *  returns the order of the letter in the alphabet
 *  if given char is not a letter, returns -1
 */
int letter_to_shift(char c) {
    if (c < 91 && c > 64) {
        return c - 64;
    } else if (c < 123 && c > 96) {
        return c - 96;
    }
    return -1;
}

/*FUNCTION TO SHIFT LETTERS*/
char shift_character(char c, int shift){
    c=(int)c;
    if (c>96 && c<123){
        if ((c+shift)>122){
            return c+(shift%26);}
        else if ((c+shift)<97){return 123+(shift%26);}
        else {return c+shift;}
    }
    else if (c>64 && c<91){
        if ((c+shift)>90){
            return c+(shift%26);}
        else if ((c+shift)<65){return 91+(shift%26);}
        else {return c+shift;}
    }
    else {return c;}
}
/* FIRST METHOD*/
char *caesar_encrypt(char *message_to_encrypt, void *shift){
    char *string;
    int i,str_size=2;
    string=(char*) malloc(sizeof(char));
    i=0;
    if(message_to_encrypt){
        while(message_to_encrypt[i]){
        string= realloc(string,(str_size++)*sizeof(char));
        if (is_letter(message_to_encrypt[i])){
        string[i]=shift_character(message_to_encrypt[i],*((int*)shift));
        }
        else{string[i]=message_to_encrypt[i];}
        i++;
        }
    string[i]='\0';
    return string; }
    else {return NULL;}
}
/* SECOND METHOD*/
char* polyalphabetic_encrypt(char* message_to_encrypt, void* key_string){
    char *string;
    int key_length,index=0,i=0,str_size=2;
    string= (char*)malloc(sizeof(char));
    key_length=get_string_length((char*)key_string);

    if(message_to_encrypt){
        while(message_to_encrypt[i]){
        index%=key_length;
        string= realloc(string,(str_size++)*sizeof(char));
        if (is_letter(message_to_encrypt[i])){
            string[i]=shift_character(message_to_encrypt[i],letter_to_shift(((char*)key_string)[index++]));}
        else {string[i]=message_to_encrypt[i];}
        i++;
        }
    string[i]='\0';
    return string; }
    else {return NULL;}
}
/* THIRD METHOD*/
char **combine_strings(char **messages, int n_messages, int grouping, char *salt){
    int salt_length,max_length,str_length;
    int j,a,salt_index=0,temp_index=0,s=1,i=0;
    char *temp,*index;
    char** result = (char**) malloc(n_messages*sizeof(char*));
    char** temp_msg = (char**) malloc(n_messages*sizeof(char*));
    for(i=0; i<n_messages; i++){
        if (messages[i]==NULL) return NULL;}

    salt_length = get_string_length(salt);
    max_length = get_longest_string_length(messages,n_messages);
    str_length = grouping*n_messages;
    while(max_length > (str_length*=s)){s++;}

    for(i=0; i<n_messages; i++){
        temp = (char*) malloc((str_length+1)*sizeof(char));
        index = messages[i];
        while(*index){
            temp[temp_index++] = *(index);                /*filling*/
            index++;                                      /*bcs it is forbidden to change the strings and initial points of pointers in the exam */
        }
        temp[str_length] = '\0';

        while(temp_index < str_length){
            salt_index %= salt_length;                  /*padding*/
            temp[temp_index++] = salt[salt_index++];
        }
        temp_index = 0;
        salt_index = 0;
        temp_msg[i]=temp;                           /*reset the pointer to use again*/
        temp = NULL;
    }
    for(j=0; j < n_messages; j++){
        temp = (char *) malloc((str_length+1)*sizeof(char));
        a=j;
        for (temp_index=0; temp_index < str_length; temp_index++){
            if (temp_index>=salt_length && temp_index%salt_length==0){a=((++a)%n_messages);}
            temp[temp_index] = ((temp_msg[a])[temp_index]);
        }
        temp[str_length] = '\0';
        result[j] = temp;
        temp = NULL;
    }

    free(temp_msg);
    return result;
}
/* TO CALL ARBITRARY METHOD, CAESAR OR POLYALPHABETIC*/
char **mycipher_encrypt(char **messages, int n_messages, int grouping, char *salt,char *(*cipher_method)(char *, void *),  void* shift){
    char **result=(char**) malloc(n_messages*sizeof(char*));
    if (cipher_method==caesar_encrypt){
        for (int i=0; i<n_messages; i++){ result[i]=caesar_encrypt(result[i],((int*)shift));}
    }
    else{
        if(cipher_method==polyalphabetic_encrypt){
            for(int i=0; i<n_messages; i++){result[i]=polyalphabetic_encrypt(result[i],salt);}
        }
    }
    return result;
}

/* TO TEST MY CODE*/
int main() {
    int cipher_method,shift_number,*shift,i=3;
    shift=i;
    char *s = "Nothing is true. Everything is permitted.";
    char* salt = "ceng";
    char ** combine;
    char** messages = (char**) malloc(3*sizeof(char*));
    char msg1[13] = "I have a pen";
    char msg2[16] = "I have an apple";
    char msg3[14] = "Uhh apple pen";
    messages[0] = msg1;
    messages[1] = msg2;
    messages[2] = msg3;
    cipher_method= caesar_encrypt;
    shift_number= 3;
    combine = combine_strings(messages, 3,4, salt);
    printf("%c", shift_character('A',5));
    return 0 ;
}
