#include "gtk_draw.h"
#include "gtk_callback.h"
#include "gtk_modal.h"

void update_file_list(res_msg_t res){
    file_list_size = res.data.file_len;
    for(int i=0;i<file_list_size;++i){
        file_list[i] = res.data.files[i];
    }
    char* text;
    int i=0;
    // printf("(%s)update: %d\n", cwd, file_list_size);
    while(i < file_list_size){
        // printf("[%d]: %s|%d|%s|%d|\n", i, file_list[i].name, file_list[i].type, file_list[i].mtime, file_list[i].size);
        text = (char*)malloc(128);
        snprintf(text, 128, "\t%-40s%-4d%4s%4d", file_list[i].name, file_list[i].type, file_list[i].mtime, file_list[i].size);
        // printf("label:%s\n", text);

        // printf("(%d).\n", i);
        gtk_label_set_text(GTK_LABEL(label_data[i]), text);
        
        if(copy_mode == 1 && strcmp(copied_file.name, "") != 0){
            char tp[MAX_PATH_LEN];
            snprintf(tp, MAX_PATH_LEN, "%s/%s", cwd, file_list[i].name);
            if(strcmp(copied_file.name, tp) == 0){
                gtk_style_class_toggle(label_data[i], "cut", TRUE);
            }else{
                gtk_style_class_toggle(label_data[i], "cut", FALSE);
            }
        }
        
        ++i;
    }

    while(i < MAX_FILE_LIST_SIZE){
        // printf("(%d) clean\n", i);
        gtk_label_set_text(GTK_LABEL(label_data[i]), "");
        ++i;
    }
}

void set_text_current_dir(char* new_cwd){
    strncpy(cwd, new_cwd, MAX_PATH_LEN);
    gtk_label_set_text(GTK_LABEL(dir_text), cwd);
}

GtkWidget *create_gtk_main_window()
{
    GtkWidget *window;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "System_Programming_Final_Project");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 800);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_exit), NULL);

    return window;
}

void css_init(){
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(
        screen,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    gtk_css_provider_load_from_path(provider, "src/resource/style.css", NULL);
    g_object_unref(provider);
}

void css_reload() {
    GtkCssProvider *css_provider;
    GdkDisplay *display;
    GdkScreen *screen;

    // 새로운 CSS Provider 생성
    css_provider = gtk_css_provider_new();

    // CSS 파일 경로를 다시 로드
    gtk_css_provider_load_from_path(css_provider, "src/resource/style.css", NULL);

    // 기본 화면을 가져오기
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    // 기존의 스타일 제공자를 교체하여 새로운 CSS 파일 적용
    gtk_style_context_add_provider_for_screen(
        screen,
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    // CSS Provider 메모리 해제
    g_object_unref(css_provider);
}


void build_layout(GtkWidget* window){
    init_modal(window);
    GtkWidget *vbox;

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // vbox :
    GtkWidget *header_hbox, *body_hbox, *footer_vbox;
    header_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(header_hbox, 1000, 50);
    // gtk_widget_set_name(header_hbox, "header_box");
    gtk_style_class_toggle(header_hbox, "header_box", TRUE);

    body_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(body_hbox, 1000, 650);
    // gtk_widget_set_name(body_hbox, "body_box");
     gtk_style_class_toggle(body_hbox, "body_box", TRUE);

    footer_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(footer_vbox, 1000, 100);
    // gtk_widget_set_name(footer_vbox, "footer_box");
     gtk_style_class_toggle(footer_vbox, "footer_box", TRUE);

    gtk_box_pack_start(GTK_BOX(vbox), header_hbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), body_hbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), footer_vbox, TRUE, TRUE, 0);

    // header_h :
    GtkWidget *btn_mkdir, *btn_mkfile, *btn_ps, *btn_quit;

    btn_mkdir = gtk_button_new_with_label("+ Directory");
    // gtk_widget_set_name(btn_mkdir, "custom_button");
     gtk_style_class_toggle(btn_mkdir, "custom_button", TRUE);

    btn_mkfile = gtk_button_new_with_label("+ File");
    btn_ps = gtk_button_new_with_label("Show Process");

    btn_quit = gtk_button_new_with_label("quit");

    g_signal_connect(btn_mkdir, "clicked", G_CALLBACK(g_callback_mkdir_popup_open), NULL);
    g_signal_connect(btn_quit, "clicked", G_CALLBACK(g_callback_quit), NULL);

    gtk_box_pack_start(GTK_BOX(header_hbox), btn_mkdir, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(header_hbox), btn_mkfile, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(header_hbox), btn_ps, FALSE, FALSE, 5);
    gtk_box_pack_end(GTK_BOX(header_hbox), btn_quit, FALSE, FALSE, 5);

    // body_h :
    GtkWidget* sidebar_vbox, *content_vbox;
    sidebar_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(sidebar_vbox, 150, 650);
    // gtk_widget_set_name(sidebar_vbox, "side_box");
     gtk_style_class_toggle(sidebar_vbox, "side_box", TRUE);

    content_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(content_vbox, 850, 650);
    // gtk_widget_set_name(content_vbox, "content_box");
     gtk_style_class_toggle(content_vbox, "content_box", TRUE);

    gtk_box_pack_start(GTK_BOX(body_hbox), sidebar_vbox, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(body_hbox), content_vbox, TRUE, TRUE, 5);

    // side_box_v :
    GtkWidget* btn_mvdir_desktop;
    GtkWidget* btn_mvdir_downloads;
    GtkWidget* btn_mvdir_documents;

    // content_box_v :
    GtkWidget* dir_vbox, *search_vbox, *list_vbox;
    dir_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(dir_vbox, 850, 50);
    // gtk_widget_set_name(dir_vbox, "dir_box");
     gtk_style_class_toggle(dir_vbox, "dir_box", TRUE);

    search_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(search_vbox, 850, 50);
    // gtk_widget_set_name(search_vbox, "search_box");
     gtk_style_class_toggle(search_vbox, "search_box", TRUE);

    GtkWidget* scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(scrolled_window, 850, 550);

    gtk_box_pack_start(GTK_BOX(content_vbox), dir_vbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(content_vbox), search_vbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(content_vbox), scrolled_window, TRUE, TRUE, 0);

    // dir_box_h
    dir_text = gtk_label_new(cwd);
    //  gtk_widget_set_name(dir_text, "dir_text");
     gtk_style_class_toggle(dir_text, "dir_text", TRUE);
    gtk_box_pack_start(GTK_BOX(dir_vbox), dir_text, FALSE, FALSE, 0);

    // list_box
    list_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    // gtk_widget_set_name(list_vbox, "list_box");
     gtk_style_class_toggle(list_vbox, "list_box", TRUE);

    gtk_container_add(GTK_CONTAINER(scrolled_window), list_vbox);
    int* index;
    printf("label build..\n");
    for(int i=0;i<MAX_FILE_LIST_SIZE;++i){
        GtkWidget* event_box = gtk_event_box_new();
        // gtk_widget_set_name(event_box, "file_info_text");
         gtk_style_class_toggle(event_box, "file_item_box", TRUE);

        label_data[i] = gtk_label_new("test label");
        gtk_style_class_toggle(label_data[i], "file_item_text", TRUE);
        gtk_label_set_xalign(GTK_LABEL(label_data[i]), 0.0);
        gtk_container_add(GTK_CONTAINER(event_box), label_data[i]);

        index = g_new(int, 1);
        *index = i;
        g_object_set_data(G_OBJECT(event_box), "index", index);

        g_signal_connect(event_box, "button-press-event", G_CALLBACK(g_callback_item_clicked), NULL);
        g_signal_connect(event_box, "enter-notify-event", G_CALLBACK(on_enter_notify), NULL);
        g_signal_connect(event_box, "leave-notify-event", G_CALLBACK(on_leave_notify), NULL);

        gtk_box_pack_start(GTK_BOX(list_vbox), event_box, FALSE, FALSE, 10);
    }
    
    // footer_h :
    GtkWidget* clipboard_vbox;
    clipboard_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    // gtk_widget_set_name(clipboard_vbox, "clipboard_box");
     gtk_style_class_toggle(clipboard_vbox, "clipboard_box", TRUE);
    gtk_box_pack_start(GTK_BOX(footer_vbox), clipboard_vbox, TRUE, TRUE, 0);

    copied_file_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(clipboard_vbox), copied_file_label, TRUE, TRUE, 0);
    
    dialog_label = gtk_label_new("");
    // gtk_widget_set_name(dialog_label, "dialog_label");
     gtk_style_class_toggle(dialog_label, "dialog_label", TRUE);
    gtk_box_pack_start(GTK_BOX(footer_vbox), dialog_label, TRUE, TRUE, 0);

    gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);
}

gboolean clear_dialog_text(gpointer data){
    gtk_label_set_text(dialog_label, "");
    return FALSE;
}

void show_dialog_text(char* msg){
    gtk_label_set_text(dialog_label, msg);
    g_timeout_add(3000, clear_dialog_text, NULL);
}

void gtk_style_class_toggle(GtkWidget* widget, char* classname, gboolean flag){
    GtkStyleContext *context = gtk_widget_get_style_context(widget);

    if (flag == TRUE) {
        gtk_style_context_add_class(context, classname);
    } else {
        gtk_style_context_remove_class(context, classname);
    }
    gtk_widget_queue_draw(widget);
}