#include "gtk_utils.h"

int gtk_exit(GtkWidget* widget, gpointer data){
    // msgctl(req_msg_q_id, IPC_RMID, (struct msqid_ds*)NULL);
    msgctl(res_msg_q_id, IPC_RMID, (struct msqid_ds*)NULL);

    gtk_main_quit();

    return 0;
}

gboolean on_enter_notify(GtkWidget *widget, GdkEventCrossing *event, gpointer user_data) {
    gtk_widget_set_state_flags(widget, GTK_STATE_FLAG_PRELIGHT, TRUE);
    return FALSE;
}

gboolean on_leave_notify(GtkWidget *widget, GdkEventCrossing *event, gpointer user_data) {
    gtk_widget_unset_state_flags(widget, GTK_STATE_FLAG_PRELIGHT);
    return FALSE;
}

void on_popup_close(GtkWidget *widget, gpointer data) {
    GtkWidget *popup = GTK_WIDGET(data);
    gtk_widget_hide(popup);  // gtk_widget_destroy 시 위젯이 완전히 삭제되기 때문에 hide 사용 
}


