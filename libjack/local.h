#ifndef __jack_libjack_local_h__
#define __jack_libjack_local_h__

#include "jack/internal.h"

/* Client data structure, in the client address space. */
struct _jack_client {

    jack_control_t        *engine;
    jack_client_control_t *control;
    jack_shm_info_t        engine_shm;
    jack_shm_info_t        control_shm;

    struct pollfd*  pollfd;
    int             pollmax;
    int             graph_next_fds_array[JACK_MAX_CLIENTS];
    int             request_fd;
    int	            process_pipe[2];

    volatile int    chain_override;
    volatile int    last_wakeup_chain;

    /* these two are copied from the engine when the 
     * client is created.
    */

    jack_port_type_id_t n_port_types;
    jack_shm_info_t*    port_segment;

    JSList *ports;
    JSList *ports_ext;

    pthread_t thread;
    char fifo_prefix[PATH_MAX+1];
    void (*on_shutdown)(void *arg);
    void *on_shutdown_arg;
    void (*on_info_shutdown)(jack_status_t, const char*, void *arg);
    void *on_info_shutdown_arg;
    char thread_ok : 1;
    char rt_thread_ok : 1;
    char first_active : 1;
    pthread_t thread_id;
    char name[JACK_CLIENT_NAME_SIZE];

#ifdef JACK_USE_MACH_THREADS
    /* specific ressources for server/client real-time thread communication */
    mach_port_t clienttask, bp, serverport, replyport;
    trivial_message  message;
#endif
    pthread_t process_thread;
    pthread_mutex_t process_mutex;
    pthread_cond_t  process_wakeup;

    /* callbacks 
     */
    JackProcessCallback process;
    void *process_arg;
    JackThreadInitCallback thread_init;
    void *thread_init_arg;
    JackBufferSizeCallback bufsize;
    void *bufsize_arg;
    JackSampleRateCallback srate;
    void *srate_arg;
    JackPortRegistrationCallback port_register;
    void *port_register_arg;
    JackPortConnectCallback port_connect;
    void *port_connect_arg;
    JackGraphOrderCallback graph_order;
    void *graph_order_arg;
    JackXRunCallback xrun;
    void *xrun_arg;
    JackSyncCallback sync_cb;
    void *sync_arg;
    JackTimebaseCallback timebase_cb;
    void *timebase_arg;
    JackFreewheelCallback freewheel_cb;
    void *freewheel_arg;
    JackClientRegistrationCallback client_register;	
    void *client_register_arg;
	JackThreadCallback thread_cb;	
    void *thread_cb_arg;

    /* external clients: set by libjack
     * internal clients: set by engine */
    int (*deliver_request)(void*, jack_request_t*); /* JOQ: 64/32 bug! */
    void *deliver_arg;

};

extern int jack_client_deliver_request (const jack_client_t *client,
					jack_request_t *req);
extern jack_port_t *jack_port_new (const jack_client_t *client,
				   jack_port_id_t port_id,
				   jack_control_t *control);

extern void *jack_zero_filled_buffer;

extern void jack_set_clock_source (jack_timer_type_t);

#endif /* __jack_libjack_local_h__ */
