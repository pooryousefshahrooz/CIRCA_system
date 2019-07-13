#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#define EVENT_ID_LENGTH  20
#define PREFIX_LENGTH  20
#define TIME_STAMP_LENGTH 20

/* a list of prefixes  */
struct update_prefix_list{
    char * prefix[PREFIX_LENGTH];
    struct update_prefix_list * next;
};

/* a list of root cause event ids  */
struct list_of_event_ids{
    char * event_id[EVENT_ID_LENGTH];
    struct list_of_event_ids * next;
};


/*This data structure will save the state of root cause events if they have converged or not.*/
struct converged{
    char * event_id[EVENT_ID_LENGTH];
    bool converged_value;
    struct list_of_event_ids concurrent_events;
    struct converged* next;
};

struct peer{
    int AS_number;
    char * e_id[20];
    struct peer * next;
};

/* A list of peers*/
struct peer_list{
    struct peer * peer;
    char * e_id[20];
    struct peer_list * next;
};

struct neighbours_sent_to{
    char * event_id[EVENT_ID_LENGTH];
    struct peer_list* peer_list;
    struct neighbours_sent_to * next;
};

/*for building the message dag and find which timestamp has cause which timestamp== which message caused which message*/
struct cause{
    char *sending_timestamp[TIME_STAMP_LENGTH];
    char *event_id[EVENT_ID_LENGTH];
    struct peer *received_from_peer;
    char * as_path;
    char *received_timestamp[EVENT_ID_LENGTH];
    struct cause* next;
};


/* this structure is for saving the list of received prefixes which
 * we can use if  all the prefixes in the list of prefixes for
 * the timestamp have been fizzled or not */
struct time_stamp_ds{
    char  *time_stamp[EVENT_ID_LENGTH];
    char *event_id[EVENT_ID_LENGTH];
    struct update_prefix_list *upl;
    struct update_prefix_list *saved_prefixes;
    struct time_stamp_ds * next;

};

/* a list of timestamps  */
struct list_of_time_stamp{
    struct time_stamp_ds tsp;
    struct list_of_time_stamp * next;
};

/* this data strucure is used for tracking the sent timestamps */
struct sent{
    struct  list_of_time_stamp lts;
    char time_stamp[TIME_STAMP_LENGTH];
    char event_id[EVENT_ID_LENGTH];
    struct peer * sent_to_peer;
    struct sent * next;
};


void add_prefix_to_prefix_list(struct update_prefix_list** head_ref,char *prefix)
{
    struct update_prefix_list * new_node = (struct update_prefix_list*) malloc(sizeof(struct update_prefix_list));
    strncpy(new_node -> prefix ,prefix, 20);
    new_node->next = (*head_ref);
    (*head_ref) = new_node;
}

void print_update_prefix_list(struct update_prefix_list ** head_ref)
{
    struct update_prefix_list * temp = (*head_ref);
    while(temp != NULL)
    {
        printf("this is the prefix %s", temp -> prefix);
        temp = temp -> next;
    }
}

void print_time_stamp(struct time_stamp_ds ** head_ref)
{
    struct time_stamp_ds * temp = (*head_ref);
    while(temp != NULL)
    {
        printf("this is the event id %s \n", temp -> event_id);
        struct update_prefix_list * my_temp = temp -> upl ;
        printf("********** going to print the prefix list ******** \n");
        while(my_temp != NULL)
        {
            printf("this is the prefix  %s \n", my_temp -> prefix);
            my_temp = my_temp -> next;
        }
        temp = temp -> next;
    }
}


void add_new_time_stamp(struct time_stamp_ds** head_ref,char * in_event_id,char * in_time_stamp_id,long AS_owner_id,struct update_prefix_list* pl)

{
    struct time_stamp_ds * new_node = (struct time_stamp_ds*) malloc(sizeof(struct time_stamp_ds));
    strncpy(new_node -> event_id ,in_event_id, 20);
    strncpy(new_node -> time_stamp ,in_time_stamp_id, 20);
    new_node->upl = pl;
    new_node->saved_prefixes = pl;
    new_node->next = (*head_ref);
    (*head_ref) = new_node;

}

char *  delete_prefix_from_update_prefix_list(struct time_stamp_ds** head_ref,char * in_prefix,char passed_event_id[],char passed_time_stamp[])
{
    struct time_stamp_ds * temp = (*head_ref);
    while(temp != NULL)
    {
        printf("this is the event id %s \n", temp -> event_id);
        struct update_prefix_list * my_temp = temp -> upl ;
        printf("********** going to check the prefix list ******** \n");
        while(my_temp != NULL)
            {
                printf("this is the prefix  %s %s \n", my_temp -> prefix,in_prefix);
                if(strcmp (my_temp -> prefix,in_prefix)==0)
                    {
                        temp -> upl = my_temp->next;   // Changed head
                        free(my_temp);
                        strncpy(passed_event_id ,temp->event_id, 20);
                        strncpy(passed_time_stamp ,temp->time_stamp, 20);
                        if (temp -> upl -> prefix == NULL)
                        {
                            printf("this is empty........  \n");
                            char *char_test[20];
//                            strncpy(char_test ,'1', 20);
//                            strncpy(passed_empty_variable ,char_test, 20);
                            return 2;
                        }
                        else {
                            int i=2;
//                            char * char_test2[20];
//                            strncpy(char_test2 ,'0', 20);
//                            strncpy(passed_empty_variable ,char_test2, 20);
                            return 1;
                        }

                        printf("this is the new node in prefix list  %s \n", temp -> upl -> prefix);
                        return 1;
                    }
                my_temp = my_temp -> next;
            }
        temp = temp -> next;
    }
    return -1;

}



void test_function(char value1[],char  value2[])
{
    strncpy(value1 ,"value1", 20);
    strncpy(value2 ,"value2", 20);
}




int  get_event_id_time_stamp(struct time_stamp_ds** head_ref,char * in_prefix,long AS_number,char passed_event_id[],char passed_time_stamp[])
{
    struct time_stamp_ds * temp = (*head_ref);
    while(temp != NULL)
    {
        struct update_prefix_list * my_temp = temp -> upl ;
        while(my_temp != NULL)
        {
            if(strcmp (my_temp -> prefix,in_prefix)==0)
            {
                strncpy(passed_event_id ,temp->event_id, 20);
                strncpy(passed_time_stamp ,temp->time_stamp, 20);
                return 1;
            }
            my_temp = my_temp -> next;
        }
        temp = temp -> next;
    }
    return -1;

}


void add_to_sent(struct sent** head_ref, char * in_time_stamp, char * event_id,struct peer* in_neighbour) /* Done */

{
    struct sent * new_node = (struct sent*) malloc(sizeof(struct sent));
    strncpy(new_node ->time_stamp ,in_time_stamp, 20);
    new_node -> sent_to_peer = in_neighbour;
    strncpy(new_node -> event_id, event_id, 20);
    new_node->next = (*head_ref);
    /* 4. move the head to point to the new node */
    (*head_ref) = new_node;
}

bool check_if_sent_is_empty(struct sent** head_ref,char *event_id) /* allmost Done! */
{
    struct sent * temp = (*head_ref);
    struct sent * prev = (struct sent *) malloc(sizeof(struct sent));
    if(temp!=NULL)
    {
        while(temp != NULL && strcmp(temp -> event_id , event_id)!=0)
        {
            prev = temp;
            temp = temp -> next;
        }
        if(temp == NULL){
            //zlog_debug("sent is empty");
            return true;
        }
        if (strcmp(temp -> event_id , event_id)==0)
        {
            return false;
        }
    }
    else
    {
        //zlog_debug("sent is empty");
        return true;
    }
}

void delete_from_sent(struct sent** head_ref,char *received_time_stamp,char *in_event_id)
{
    struct sent * temp = (*head_ref);
    struct sent * prev = (struct sent *) malloc(sizeof(struct sent));//temp -> neighbour -> local_id.s_addr == in_neighbour -> local_id.s_addr
    while(temp != NULL )
    {
        if ((strcmp(temp-> event_id ,in_event_id)==0) && (strcmp(temp-> time_stamp ,received_time_stamp)==0))//&& (strcmp(temp -> timestamp ,in_time_stamp)==0)
        {
            *head_ref = temp->next;   // Changed head
            free(temp);               // free old head
            return;
        }
        else
        {
            //zlog_debug("not equal, lets move!");
            prev = temp;
            temp = temp -> next;
        }
    }

    //zlog_debug("Could not delete event from sent b/c entry does not exist or the list is empty");


}

struct cause * getcause(struct cause** head_ref,char * received_time_stamp,char *in_event_id)/* Done */
{
    struct cause* temp = (*head_ref);
    struct cause* result = NULL;
    while(temp != NULL){
        if((strcmp(temp -> sending_timestamp ,received_time_stamp)==0)&& (strcmp(temp -> event_id ,in_event_id)==0)){
            result = temp;
            break;
        }else{
           // zlog_debug("not equal lets move");
            temp = temp-> next;
        }

    }
    return result;
}
void addcause(struct cause** head_ref,char *be_sending_time_stamp,char *causality,char *in_event_id,struct peer * peer) /* Done */

{
    struct cause* new_node = (struct cause *) malloc(sizeof(struct cause));
    strncpy(new_node -> sending_timestamp, be_sending_time_stamp, 20);
    printf("passed be_sending_time_stamp  %s!\n",be_sending_time_stamp);
    printf("passed in_event_id  %s!\n",in_event_id);
    printf("passed causality  %s!\n",causality);
    strncpy(new_node -> event_id, in_event_id, 20);
    strncpy(new_node -> received_timestamp, causality, 20);
    new_node -> received_from_peer = peer;
    new_node->next = (*head_ref);
    (*head_ref) = new_node;

}


void insert_in_converged(struct converged ** head_ref, char * in_event_id){
    struct converged* temp = (*head_ref);

    while(temp != NULL){
        if(strcmp(temp -> event_id , in_event_id)==0){
            printf("\nthe event_id already exists \n");
            return;
        }else{
            temp = temp -> next;
        }
    }
    struct converged * new_node = (struct converged *) malloc(sizeof(struct converged));
    strcpy(new_node->event_id,in_event_id);
    new_node -> converged_value = false;

    new_node->next = (*head_ref);
    (*head_ref) = new_node;
}


void set_converged_value_true(struct converged ** head_ref, char * in_event_id){
    int found = 0;
    struct converged * temp = (*head_ref);
    while(temp != NULL){
        if(strcmp(temp -> event_id , in_event_id)==0){
            temp -> converged_value = true;
            found = 1;
            break;
        }else{
            temp = temp ->next;
        }
    }
    if(found == 1){
        printf(" Successfully set the converged value to true of the event with event_id %s \n", temp -> event_id );
    }else{
        printf("the event with event_id %s does not exixt\n", in_event_id );
    }
}


int get_converged_value(struct converged ** head_ref, char * in_event_id){
    struct converged * temp = (*head_ref);
    while(temp != NULL){
        if(strcmp(temp -> event_id , in_event_id)==0){

            if(temp -> converged_value == true){
                return 1;
            }else if(temp -> converged_value == false){
                return 0;
            }

        }else{
            temp = temp -> next;
        }
    }
    return -1;
}



void add_to_neighbours_sent_to_of_an_event(struct neighbours_sent_to** head_ref,char *in_event_id,struct peer_list * in_peer_list) /* Done */
{
    struct neighbours_sent_to * new_node3 = (struct neighbours_sent_to *) malloc(sizeof(struct neighbours_sent_to));
    strncpy(new_node3 -> event_id, in_event_id, 20);
    //new_node3 -> key_prefix= in_prefix;
    new_node3 -> peer_list = in_peer_list;
    new_node3 -> next = (*head_ref);
    (*head_ref) = new_node3;


}

void print_peer_list(struct peer_list ** head_ref){
    struct peer_list * temp = (*head_ref);
    printf("Lets print the list \n");
    while(temp != NULL){
        printf("this is the local as of the peer %d \n",temp->peer->AS_number);
        temp = temp -> next;
    }
    printf("print peer list ends here \n");

}


void print_neighbours_of_a_prefix(struct neighbours_sent_to ** head_ref)
{
    struct neighbours_sent_to * temp = (*head_ref);
    printf("******* going to print neighbours of a event  ******** \n");
    while(temp != NULL){
        printf("this is the event id %s \n", temp -> event_id);
        print_peer_list(&(temp -> peer_list));
        temp = temp -> next;
    }
    //zlog_debug("print neighbours of a prefix ends here ");

}

struct neighb * get_neighbours_sent_to(struct neighbours_sent_to** head_ref, char *in_event_id){
    struct neighbours_sent_to * temp = (*head_ref);
    struct neighbours_sent_to * result = NULL;
    while(temp != NULL){
        // zlog_debug("we are going to compare %s and %s to get the peer list",temp -> key_prefix,in_prefix );
        if(strcmp (temp -> event_id,in_event_id)==0){
            result = temp;
            break;
        }else{
            temp = temp -> next;
        }
    }
    return result;
}


void remove_neighbours_of_a_prefix(struct neighbours_of_a_prefix** head_ref,char * in_event_id)
{
    struct neighbours_sent_to * temp = (*head_ref);
    struct neighbours_sent_to * result = NULL;
    while(temp != NULL){
    // zlog_debug("we are going to compare %s and %s to get the peer list",temp -> key_prefix,in_prefix );
    if(strcmp (temp -> event_id,in_event_id)==0){
    *head_ref = temp->next;   // Changed head
    free(temp);               // free old head
    return;
    break;
    }else{
    temp = temp -> next;
    }
    }
    return;
}


void add_to_peer_list(struct peer_list ** head_ref,struct peer * in_peer){
    struct peer_list* new_node4 = (struct peer_list *)malloc(sizeof(struct peer_list));
    new_node4 -> peer = in_peer;
    new_node4 -> next = (*head_ref);
    (*head_ref) = new_node4;
}


void add_peer_to_neighbors_sent_to(struct neighbours_sent_to** head_ref,char *in_event_id,struct peer * in_peer)
{
    struct neighbours_sent_to * temp = (*head_ref);

    while(temp != NULL)
    {
        if(strcmp(temp->event_id,in_event_id)==0) {
            struct peer_list* new_node4 = (struct peer_list *)malloc(sizeof(struct peer_list));
            new_node4 -> peer = in_peer;
            new_node4 -> next = (*&(temp->peer_list));
            (*&(temp->peer_list)) = new_node4;
            return;
            }

        temp = temp->next;
    }
    if(temp ==NULL)
    {
        struct neighbours_sent_to * new_node = (struct neighbours_sent_to *) malloc(sizeof(struct neighbours_sent_to));
        strcpy(new_node -> event_id ,in_event_id);

        /* adding peer to a null peer list */


        /* end */
        struct peer_list* new_node4 = (struct peer_list *)malloc(sizeof(struct peer_list));
        new_node4 -> peer = in_peer;
        new_node4 -> next = (*&(new_node->peer_list));
        new_node->peer_list = new_node4;

        new_node -> next = (*head_ref);
        (*head_ref) = new_node;

    }
}
void add_peer(struct peer ** head_ref,int AS_number) {
    struct peer *new_node4 = (struct peer *) malloc(sizeof(struct peer));
    new_node4->AS_number = AS_number;
    new_node4->next = (*head_ref);
    (*head_ref) = new_node4;
}

struct peer_list * get_list_of_peers_sent_to(struct neighbours_sent_to** head_ref,char *in_event_id)
{
    struct neighbours_sent_to * temp = (*head_ref);
    struct neighbours_sent_to * result = NULL;
    while(temp != NULL){
        // zlog_debug("we are going to compare %s and %s to get the peer list",temp -> key_prefix,in_prefix );
        if(strcmp (temp -> event_id,in_event_id)==0){
            result = temp;
            break;
        }else{
            temp = temp -> next;
        }
    }
    return result;

}
int main() {

    /* Start with the empty list */
    struct time_stamp_ds* head = NULL;
    struct update_prefix_list* prefix_list_head = NULL;
    struct sent* sent_head = NULL;
    struct cause* cause_head = NULL;
    struct  converged* converged_head = NULL;
    struct neighbours_sent_to* neighbours_sent_to_head = NULL;
    struct peer* peer_head = NULL;
    char * in_event_id[20];
    char * my_prefix[20];
    char * in_time_stamp_id[20];
    long AS_owner_id =1234;

    char * passed_time_stamp[20];
    char * passed_event_id[20];
    char * passed_empty_variable[20];
    strncpy(passed_empty_variable ,"empty", 20);
    char * empty_list[20];
    char * full_list[20];

    strncpy(empty_list ,"empty", 20);
    strncpy(full_list ,"full", 20);

    strncpy(my_prefix ,"10.10.0.0/24", 20);
    add_prefix_to_prefix_list(&prefix_list_head,my_prefix);
    strncpy(my_prefix ,"100.100.0.0/24", 20);

    add_prefix_to_prefix_list(&prefix_list_head,my_prefix);
    //print_update_prefix_list(&prefix_list_head);

    strncpy(in_time_stamp_id ,"time_stamp_id,543", 20);


    strncpy(in_event_id ,"event_id,1234", 20);


    add_new_time_stamp(&head,in_event_id,in_time_stamp_id,AS_owner_id,prefix_list_head);


   // print_time_stamp(&head);

    printf("Hello, World!\n");

    struct time_stamp_ds* head2 = NULL;
    struct update_prefix_list* prefix_list_head2 = NULL;

    strncpy(my_prefix ,"20.20.0.0/24", 20);
    add_prefix_to_prefix_list(&prefix_list_head2,my_prefix);
    strncpy(my_prefix ,"200.200.0.0/24", 20);

    add_prefix_to_prefix_list(&prefix_list_head2,my_prefix);
    //print_update_prefix_list(&prefix_list_head2);

    strncpy(in_time_stamp_id ,"time_stamp_id,987", 20);


    strncpy(in_event_id ,"event_id,56789", 20);


    add_new_time_stamp(&head,in_event_id,in_time_stamp_id,AS_owner_id,prefix_list_head2);


    print_time_stamp(&head);
    printf("We added second time stamp!\n");
//    int ret;
//    ret =delete_prefix_from_update_prefix_list(&head,my_prefix,&passed_event_id,&passed_time_stamp);
//    if(ret==2){
//        printf("1. we found our prefix in time stamp list! %s %s \n",passed_event_id,passed_time_stamp);
//        //print_time_stamp(&head);
//        printf("1.prefix list is empty");
////        if(strcmp (passed_empty_variable,'1')==0)
////            printf("prefix list is empty");
////        else
////            printf("2.prefix list is not empty");
//    }
//    if (ret==1){
//        printf("1. we found our prefix in time stamp list! %s %s \n",passed_event_id,passed_time_stamp);
//        //print_time_stamp(&head);
//        printf("1.prefix list is not empty");
//    }
//    if (ret<0){
//        printf("1.we did not find our prefix in time stamp list %s %s \n",passed_event_id,passed_time_stamp);
//    }
//
//
//    strncpy(my_prefix ,"20.20.0.0/24", 20);
//    ret =delete_prefix_from_update_prefix_list(&head,my_prefix,&passed_event_id,&passed_time_stamp);
//    if(ret==2){
//        printf("2. we found our prefix in time stamp list! %s %s \n",passed_event_id,passed_time_stamp);
//        //print_time_stamp(&head);
//        printf("2.prefix list is empty");
////        if(strcmp (passed_empty_variable,'1')==0)
////            printf("prefix list is empty");
////        else
////            printf("2.prefix list is not empty");
//    }
//    if (ret==1){
//        printf("2. we found our prefix in time stamp list! %s %s \n",passed_event_id,passed_time_stamp);
//        //print_time_stamp(&head);
//        printf("2.prefix list is not empty");
//    }
//    if (ret<0){
//        printf("2.we did not find our prefix in time stamp list %s %s \n",passed_event_id,passed_time_stamp);
//    }
//
//
////    char * value1[20];
////    char * value2[20];
////    printf("values\n");
////    test_function(&value1,&value2);
////    printf("values %s %s \n",value1,value2);
//    printf("\n After all of the things .... \n");
//    print_time_stamp(&head);
//
//    strncpy(my_prefix ,"100.100.0.0/24", 20);
//    ret = get_event_id_time_stamp(&head,my_prefix,12,&passed_event_id,&passed_time_stamp);
//    if (ret >0)
//        printf("the received event id and time stamp for prefix are %s %s %s \n",my_prefix,passed_event_id,passed_time_stamp);
//    if (ret <0)
//        printf("We do not get anything for the received event id and time stamp for prefix are %s %s %s \n",my_prefix,passed_event_id,passed_time_stamp);

    strncpy(in_time_stamp_id ,"time_stamp_id,543", 20);
    struct peer * p;

    add_to_sent(&sent_head, in_time_stamp_id, in_event_id,p);
    delete_from_sent(&sent_head,in_time_stamp_id,in_event_id);
    bool sent_checking;
    sent_checking = check_if_sent_is_empty(&sent_head,in_event_id);
    if (sent_checking)
    {
        printf("Sent is empty!\n");
    }
    else
        printf("Sent is not empty!\n");

    struct timeval start, end;
    long mtime, seconds, to_be_sent_time_stamp;
    gettimeofday(&start, NULL);
    to_be_sent_time_stamp = start.tv_usec;
    printf("\n time in microseconds .... %ld \n",to_be_sent_time_stamp);
    char * char_to_be_sent_time_stamp[20];
    sprintf(char_to_be_sent_time_stamp, "%u", to_be_sent_time_stamp);
    //strncpy(char_to_be_sent_time_stamp ,to_be_sent_time_stamp, 20);

    char * char_my_router_id[20];
    sprintf(char_my_router_id, "%u", 12);
    strcat(char_to_be_sent_time_stamp, ",");
    strcat(char_to_be_sent_time_stamp, char_my_router_id);

    printf("Our time stamp: %s \n", char_to_be_sent_time_stamp);

    addcause(&cause_head,char_to_be_sent_time_stamp,in_time_stamp_id,in_event_id,p);
    struct cause * our_cause;
    our_cause = getcause(&cause_head,char_to_be_sent_time_stamp,in_event_id);
    printf("The cause of %s is %s! \n",char_to_be_sent_time_stamp,our_cause->received_timestamp);

    insert_in_converged(&converged_head, in_event_id);
    int convergence_status = get_converged_value(&converged_head, in_event_id);
    if (convergence_status==1)
        printf("1.event id %s is converged! \n",in_event_id);
    if (convergence_status==0)
        printf("1.event id %s is not  converged! \n",in_event_id);
    if (convergence_status<0)
        printf("1.event id %s does not exist ! \n",in_event_id);

    set_converged_value_true(&converged_head,in_event_id);
    int convergence_status2 = get_converged_value(&converged_head, in_event_id);
    if (convergence_status2==1)
        printf("2.event id %s is converged! \n",in_event_id);
    if (convergence_status2==0)
        printf("2.event id %s is converged! \n",in_event_id);
    if (convergence_status2<0)
        printf("2.event id %s does not exist ! \n",in_event_id);

    printf("Done! \n");


    struct peer_list* peer_list_head = NULL;
    add_peer(&peer_head,1345678);
    add_peer_to_neighbors_sent_to(&neighbours_sent_to_head, in_event_id,peer_head);
    print_neighbours_of_a_prefix(&(neighbours_sent_to_head));

    add_peer(&peer_head,91011);
    add_peer_to_neighbors_sent_to(&neighbours_sent_to_head, in_event_id,peer_head);
    print_neighbours_of_a_prefix(&(neighbours_sent_to_head));

    struct neighbours_sent_to * sent_to_peers = (struct neighbours_sent_to *) malloc (sizeof(struct neighbours_sent_to));
    sent_to_peers = get_list_of_peers_sent_to(&neighbours_sent_to_head,in_event_id);

    if (sent_to_peers != NULL)
    {
        struct peer_list * my_temp = sent_to_peers -> peer_list ;
        while(my_temp != NULL)
        {
            printf("this   %ld is the AS number of the peer we are sending CONVERGENCE MSG to \n", my_temp -> peer -> AS_number);
            my_temp = my_temp -> next;
        }
    }

    strncpy(in_event_id ,"event_id,888888", 20);
    add_peer_to_neighbors_sent_to(&neighbours_sent_to_head, in_event_id,peer_head);
    print_neighbours_of_a_prefix(&(neighbours_sent_to_head));


    printf("Done 2! \n");

//    for(int sx=1; sx<=3; sx++) {
//
//        gettimeofday(&start, NULL);
//        printf("Hello world");
//        gettimeofday(&end, NULL);
//
//        printf("Elapsed time: %ld microsecons\n", start.tv_usec);
//    }
//
//    struct timespec tstart={0,0}, tend={0,0};
//    clock_gettime(CLOCK_MONOTONIC, &tstart);
//    for(int sx=1; sx<=30; sx++) {
//
//        gettimeofday(&start, NULL);
//        printf("Hello world \n");
//        gettimeofday(&end, NULL);
//    }
//    clock_gettime(CLOCK_MONOTONIC, &tend);
//    printf("some_long_computation took about %.5f seconds\n",
//           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
//           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
//    printf("some_long_computation took about %.5f seconds\n",
//           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) * 1000000000);
    return 0;

}