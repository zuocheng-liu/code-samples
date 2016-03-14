#ifndef ITEM_H
#define ITEM_H
class Item {
    public :

        void item_free(item *it);
int item_size_ok(char *key, int flags, int nbytes); 
 
        item *item_alloc(char *key, int flags, rel_time_t exptime, int nbytes); 
            private :
        Item *next;
        Item *prev;
        Item *h_next;    /* hash chain next */
        rel_time_t      time;       /* least recent access */
        rel_time_t      exptime;    /* expire time */
        int             nbytes;     /* size of data */
        unsigned short  refcount;
        unsigned char   nsuffix;    /* length of flags-and-length string */
        unsigned char   it_flags;   /* ITEM_* above */
        unsigned char   slabs_clsid;/* which slab class we're in */
        unsigned char   nkey;       /* key length, w/terminating null and padding */
        void * end[0];
        /* then null-terminated key */
        /* then " flags length\r\n" (no terminating null) */
        /* then data with terminating \r\n (no terminating null; it's binary!) */
};

class ItemList {
    
};
#endif
