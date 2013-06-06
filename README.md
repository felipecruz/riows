# rioev

A thin wrapper around Epoll and Kqueue.

Very simple API: 5 functions (rioev_init, rioev_add, rioev_del, rioev_mod, rioev_poll)
And a `ITERATE` macro facility.

```C
    
    int pipes[2];
    int status = pipe(pipes);
    int r = pipes[0];
    int w = pipes[1];
    rioev_t *rioev = rioev_init ();

    rioev_add (rioev, r, RIOEV_IN);
    rioev_add (rioev, w, RIOEV_OUT);

    write (w, "a", 1);

    ITERATE(rioev, 1)
        EVENT_LOOP(rioev)
        {
            ENSURE (0 < GET_FD(ev));
            if (r == GET_FD(ev))
                ENSURE (IS_RIOEV_IN(ev));
            if (w == GET_FD(ev))
                ENSURE (IS_RIOEV_OUT(ev));
        END_LOOP
        do_other_stuff_before_another_poll ();
    END_ITERATE
```
