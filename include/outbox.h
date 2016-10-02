#ifndef _OUTBOX_H_
#define _OUTBOX_H_
typedef struct mqtt_outbox {
    char *buffer;
    int length;
    int is_pending;
    int msg_id;
    int msg_type;
    int expired_tick;
    struct mqtt_outbox *next;
} mqtt_outbox;

mqtt_outbox *ob_create(char *data, int len, int msg_id);
mqtt_outbox *ob_find_last(mqtt_outbox *ob);
mqtt_outbox *ob_find(mqtt_outbox *ob, int msg_id, mqtt_outbox *parent);
mqtt_outbox *ob_push(mqtt_outbox *ob, char *data, int len, int msg_id);
mqtt_outbox *ob_pop(mqtt_outbox *ob, char *data, int *len, int *msg_id, int is_pending);
mqtt_outbox *ob_delete(mqtt_outbox *ob, int msg_id);
#endif
