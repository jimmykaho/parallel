#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <tbb/tbb.h>
#include <openssl/md5.h>

const char* chars="0123456789";

// tests if a hash matches a candidate password
int test(const char* passhash, const char* passcandidate) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    
    MD5((unsigned char*)passcandidate, strlen(passcandidate), digest);
    
    char mdString[34];
    mdString[33]='\0';
    for(int i=0; i<16; i++) {
        sprintf(&mdString[i*2], "%02x", (unsigned char)digest[i]);
    }
    return strncmp(passhash, mdString, strlen(passhash));
}

// maps a PIN to a string
void genpass(long passnum, char* passbuff) {
    passbuff[8]='\0';
    int charidx;
    int symcount=strlen(chars);
    for(int i=7; i>=0; i--) {
        charidx=passnum%symcount;
        passnum=passnum/symcount;
        passbuff[i]=chars[charidx];
    }
}

class PassFinder{
    private:
    const char* passhash;

    public:
    PassFinder (const char* passhash_ptr){
        passhash = passhash_ptr;
    }
    void operator()(const tbb::blocked_range<long>& r) const{
        for (int i=r.begin(); i!=r.end(); i++){
            char passmatch[9];
            genpass(i, passmatch);

            // check if they match, if so, print out found statement
            if (test(passhash, passmatch)==0){
                printf("found: %s\n", passmatch);
            }
        }	
    }
};

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: %s <password hash>\n",argv[0]);
        return 1;
    }

    //char passmatch[9];
    //long currpass=0;
    //char foundmatch[9];

    //#pragma omp parallel for private(passmatch) shared(foundmatch)
    //for(; currpass<99999999; ++currpass){
        //genpass(currpass,passmatch);
        //if (test(argv[1], passmatch)==0){
            //strncpy(foundmatch, passmatch, 9);
        //}
    //}
    //printf("found: %s\n",foundmatch);
    
    PassFinder pf(argv[1]);
    parallel_for(tbb::blocked_range<long>(0, 100000000), pf);

    return 0;
}
