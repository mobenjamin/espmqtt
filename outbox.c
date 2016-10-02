/*
* @Author: Tuan PM
* @Date:   2016-10-02 09:45:51
* @Last Modified by:   Tuan PM
* @Last Modified time: 2016-10-02 11:08:14
*/

#include "mqtt_outbox.h"

mqtt_outbox *ob_create(char *data, int len, int msg_id)
{
    mqtt_outbox *ob = (mqtt_outbox *) malloc(sizeof(mqtt_outbox));
    memset(ob, 0, sizeof(mqtt_outbox));
    if (data != NULL) {
        ob->buffer = malloc(len);
        ob->len = len;
        ob->msg_id = msg_id;
        memcpy(ob->buffer, data, len);
    }
    return ob;
}
mqtt_outbox *ob_find_last(mqtt_outbox *ob)
{
    mqtt_outbox *last_ob = ob;
    while (last_ob->next != NULL) {
        last_ob = last_ob->next;
    }
    return last_ob;
}

mqtt_outbox *ob_find(mqtt_outbox *ob, int msg_id, mqtt_outbox *parent)
{
    mqtt_outbox *found_ob = ob, *parent_ob = NULL;
    while (found_ob->next != NULL) {
        parent_ob = found_ob;
        found_ob = found_ob->next;
        if(found_ob->msg_id == msg_id) {
            parent = parent_ob;
            return found_ob;
        }
    }
    return NULL;
}

mqtt_outbox *ob_push(mqtt_outbox *ob, char *data, int len, int msg_id)
{
    mqtt_outbox *last_ob = ob_find_last(ob);
    if(last_ob != NULL) {
        last_ob->next = ob_create(data, len, msg_id);
        return last_ob->next;
    }
    return NULL;
}

mqtt_outbox *ob_pop(mqtt_outbox *ob, char *data, int *len, int *msg_id, int is_pending)
{
    mqtt_outbox *last_ob = ob_find_last(ob);
    if (last_ob->buffer != NULL && last_ob->pending == is_pending) {
        memcpy(data, last_ob->buffer, last_ob->len);
        *len = last_ob->len;
        *msg_id = last_ob->msg_id;
        return last_ob;
    }
    return NULL;
}

mqtt_outbox *ob_delete(mqtt_outbox *ob, int msg_id)
{
    mqtt_outbox *ob_parent;
    mqtt_outbox *ob_deleted = ob_find(ob, msg_id, ob_parent);
    if (ob_deleted != NULL) {
        if (ob_deleted->buffer != NULL)
            free(ob_deleted->buffer);
        if(ob_deleted->next != NULL)
            ob_parent->next = ob_deleted->next;
        free(ob_deleted);
        return ob_parent;
    }
    return NULL;
}
