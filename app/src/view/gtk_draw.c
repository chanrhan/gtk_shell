#include "gtk_draw.h"
#include "gtk_callback.h"
#include "gtk_modal.h"
#include "view_func.h"

void update_file_list(res_msg_t res){
    if(display_mode != FILE_DISPLAY_MODE){
        return;
    }
    file_list_size = res.data.file_len;
    for(int i=0;i<file_list_size;++i){
        file_list[i] = res.data.files[i];
    }
    char* text;
    int i=0;
    while(i < file_list_size){
        if(strcmp(file_list[i].name, "..") == 0){
            gtk_label_set_text(GTK_LABEL(label_data[i]), "  ..");
            ++i;
            continue;
        }
        char time[12];
        get_time_format(file_list[i].mtime, time);

        text = (char*)malloc(128);
        snprintf(text, 128, "  %-30s%12s %4d", file_list[i].name, time, file_list[i].size);
        // printf("label:%s\n", text);
        
        // printf("(%d).\n", i);
        gtk_label_set_text(GTK_LABEL(label_data[i]), text);

        if(file_list[i].type == 8){
            gtk_style_class_toggle(label_data[i], "file", TRUE);
        }else{
            gtk_style_class_toggle(label_data[i], "file", FALSE);
        }
        
        char tp[MAX_PATH_LEN];
        snprintf(tp, MAX_PATH_LEN, "%s/%s", cwd, file_list[i].name);
        if(copy_mode == 1 && strcmp(copied_file.name, tp) == 0){
                gtk_style_class_toggle(label_data[i], "cut", TRUE);
        }else{
            gtk_style_class_toggle(label_data[i], "cut", FALSE);
        }

        if(strcmp(found_filename, tp) == 0){
            gtk_style_class_toggle(label_data[i], "find", TRUE);
        }else{
            gtk_style_class_toggle(label_data[i], "find", FALSE);
        }
        
        ++i;
    }

    while(i < MAX_FILE_LIST_SIZE){
        // printf("(%d) clean\n", i);
        gtk_label_set_text(GTK_LABEL(label_data[i]), "");
        ++i;
    }
}

void update_process_list(res_msg_t res){
    if(display_mode != PROCESS_DISPLAY_MODE){
        return;
    }
    printf("update process list\n");
    int len = res.data.file_len;
    char* buf[6]; // uid, pid, ppid, starttime, time, cmd
    char* tok;
    char text[128];

    int i=0;
    for(;i<len;++i){
        buf[0] = strtok(res.data.files[i].name, "<>");
        int k=1;
        for(;k<6;++k){
            if(buf[k] == NULL){
                break;
            }
            buf[k] = strtok(NULL, "<>");
        }
        for(;k<6;++k){
            buf[k] = (char*)malloc(16);
            strcpy(buf[k], "");
        }
        strcpy(file_list[i].name, buf[0]); // pid를 파일 name 필드에 저장 
        snprintf(text, 128, "%5s%5s%5s%5s%5s%5s", buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
        gtk_label_set_text(GTK_LABEL(label_data[i]), text);
    }
    for(;i<MAX_FILE_LIST_SIZE;++i){
        strcpy(file_list[i].name, "");
        gtk_label_set_text(GTK_LABEL(label_data[i]), "");
    }

    gtk_label_set_text(GTK_LABEL(path_tok_label[0]), "PROCESS MODE");
    for(int i=1;i<MAX_PATH_TOKEN;++i){
        gtk_label_set_text(GTK_LABEL(path_tok_label[i]), "");
    }
}

void update_path_token(){
    // printf("update path toekn\n");
    int size=0;
    char p[MAX_PATH_LEN];
    char show_text[MAX_PATH_LEN];
    int insertIdx = 0;

    strcpy(p, cwd);

    char* tok = strtok(p, "/");
    strncpy(path_tok[size], tok, MAX_PATH_LEN);
    ++size;

    while(1){
        tok = strtok(NULL, "/");
        // printf("(%d) tok:%s\n", size, tok);
        if(tok == NULL){
            break;
        }
        strncpy(path_tok[size], tok, MAX_PATH_LEN);
        ++size;
    }
    int i=0;
    while(i < size){
        insertIdx = i;
        if(size >= MAX_PATH_TOKEN){
            if(i > 2 && i < size - MAX_PATH_TOKEN + 3){
                ++i;
                continue;
            }
            
            if(i == 2){
                strcpy(show_text, "...");
            }else{
                snprintf(show_text, MAX_PATH_LEN-2, "> %s", path_tok[i]);
                insertIdx = i-(size-MAX_PATH_TOKEN);
            }
        }else{
            snprintf(show_text, MAX_PATH_LEN-2, "> %s", path_tok[i]);
        }
        
        // printf("(%d->%d) show_text:%s\n", i, insertIdx, show_text);
        gtk_label_set_text(GTK_LABEL(path_tok_label[insertIdx]), show_text);
        path_tok_map[insertIdx] = i;
        ++i;
    }
    while(i < MAX_PATH_TOKEN){
        // printf("clear:%d\n", i);
        strcpy(path_tok[i], "");
        gtk_label_set_text(GTK_LABEL(path_tok_label[i]), "");
        ++i;
    }
}

void update_current_working_directory(char* new_cwd){
    strncpy(cwd, new_cwd, MAX_PATH_LEN);
    // gtk_label_set_text(GTK_LABEL(dir_text), cwd);
    update_path_token();
}

void set_clipboard(char* text){
    gtk_label_set_text(GTK_LABEL(copied_file_label), text);
    gtk_style_class_toggle(copied_file_label, "copied", TRUE);
}

void clear_clipboard(){
    strcpy(copied_file.mtime, "");
    strcpy(copied_file.name, "");
    copied_file.size = -1;
    copied_file.type = -1;
    copy_mode = -1;
    gtk_label_set_text(GTK_LABEL(copied_file_label), "");
    gtk_style_class_toggle(copied_file_label, "copied", FALSE);
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
    GtkCssProvider *main_prvd, *modal_prvd;
    GdkDisplay *display;
    GdkScreen *screen;

    main_prvd = gtk_css_provider_new();
    modal_prvd = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(
        screen,
        GTK_STYLE_PROVIDER(main_prvd),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    gtk_style_context_add_provider_for_screen(
        screen,
        GTK_STYLE_PROVIDER(modal_prvd),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    gtk_css_provider_load_from_path(main_prvd, "src/resource/style.css", NULL);
    gtk_css_provider_load_from_path(modal_prvd, "src/resource/modal.css", NULL);
    g_object_unref(main_prvd);
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
    gtk_style_class_toggle(vbox, "container", TRUE);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // vbox :
    GtkWidget *header_hbox, *body_hbox, *footer_vbox;
    header_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(header_hbox, 1000, 50);
    // gtk_widget_set_name(header_hbox, "header_box");
    gtk_style_class_toggle(header_hbox, "header", TRUE);

    body_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(body_hbox, 1000, 650);
    // gtk_widget_set_name(body_hbox, "body_box");
     gtk_style_class_toggle(body_hbox, "body", TRUE);

    footer_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(footer_vbox, 1000, 100);
    // gtk_widget_set_name(footer_vbox, "footer_box");
     gtk_style_class_toggle(footer_vbox, "footer", TRUE);

    gtk_box_pack_start(GTK_BOX(vbox), header_hbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), body_hbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), footer_vbox, TRUE, TRUE, 0);

    // header_h :
    GtkWidget *btn_mkdir, *btn_mkfile, *btn_quit;

    btn_mkdir = gtk_button_new_with_label("+ Directory");
    // gtk_widget_set_name(btn_mkdir, "custom_button");
     gtk_style_class_toggle(btn_mkdir, "btn_mkdir", TRUE);

    btn_mkfile = gtk_button_new_with_label("+ File");
    gtk_style_class_toggle(btn_mkdir, "btn_mkfile", TRUE);
    btn_ps = gtk_button_new_with_label("Show Process");
    gtk_style_class_toggle(btn_mkdir, "btn_ps", TRUE);

    btn_quit = gtk_button_new_with_label("quit");
    gtk_style_class_toggle(btn_mkdir, "btn_quit", TRUE);

    g_signal_connect(btn_mkdir, "clicked", G_CALLBACK(g_callback_mkdir_popup_open), NULL);
    g_signal_connect(btn_mkfile, "clicked", G_CALLBACK(g_callback_file_info_open), NULL);
    g_signal_connect(btn_ps, "clicked", G_CALLBACK(g_callback_change_display_mode), NULL);
    g_signal_connect(btn_quit, "clicked", G_CALLBACK(g_callback_quit), NULL);

    // gtk_box_pack_start(GTK_BOX(header_hbox), btn_mkdir, FALSE, FALSE, 5);
    // gtk_box_pack_start(GTK_BOX(header_hbox), btn_mkfile, FALSE, FALSE, 5);
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

    gtk_box_pack_start(GTK_BOX(body_hbox), sidebar_vbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(body_hbox), content_vbox, TRUE, TRUE, 0);

    // side_box_v :
    GtkWidget* btn_mvdir_desktop;
    GtkWidget* btn_mvdir_downloads;
    GtkWidget* btn_mvdir_documents;

    btn_mvdir_desktop = gtk_button_new_with_label("Desktop");
    gtk_widget_set_size_request(btn_mvdir_desktop, 75, 50);
     gtk_style_class_toggle(btn_mvdir_desktop, "btn_mvdir", TRUE);
     gtk_style_class_toggle(btn_mvdir_desktop, "btn_desktop", TRUE);

    btn_mvdir_downloads = gtk_button_new_with_label("Downloads");
    gtk_widget_set_size_request(btn_mvdir_downloads, 75, 50);
     gtk_style_class_toggle(btn_mvdir_downloads, "btn_mvdir", TRUE);
     gtk_style_class_toggle(btn_mvdir_downloads, "btn_downloads", TRUE);

    btn_mvdir_documents = gtk_button_new_with_label("Documents");
    gtk_widget_set_size_request(btn_mvdir_documents, 75, 50);
     gtk_style_class_toggle(btn_mvdir_documents, "btn_mvdir", TRUE);
     gtk_style_class_toggle(btn_mvdir_documents, "btn_documents", TRUE);

    g_signal_connect(btn_mvdir_desktop, "clicked", G_CALLBACK(g_callback_mvdir), "/tmp/test/desktop");
    g_signal_connect(btn_mvdir_downloads, "clicked", G_CALLBACK(g_callback_mvdir), "/tmp/test/downloads");
    g_signal_connect(btn_mvdir_documents, "clicked", G_CALLBACK(g_callback_mvdir), "/tmp/test/documents");

    gtk_box_pack_start(GTK_BOX(sidebar_vbox), btn_mvdir_desktop, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sidebar_vbox), btn_mvdir_downloads, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sidebar_vbox), btn_mvdir_documents, FALSE, FALSE, 0);


    // content_box_v :
    GtkWidget* dir_hbox, *search_hbox, *list_header_label, *list_vbox;
    dir_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(dir_hbox, 850, 50);
    // gtk_widget_set_name(dir_vbox, "dir_box");
     gtk_style_class_toggle(dir_hbox, "dir_cont", TRUE);

    search_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(search_hbox, 850, 50);
    // gtk_widget_set_name(search_vbox, "search_box");
     gtk_style_class_toggle(search_hbox, "search_cont", TRUE);

    char list_header_text[128];
    snprintf(list_header_text, 128, "  %-30s %-12s%4s", "Name","Time","Size");
    list_header_label = gtk_label_new(list_header_text);
    gtk_label_set_xalign(GTK_LABEL(list_header_label), 0.0);
    gtk_style_class_toggle(list_header_label, "list_header", TRUE);
    gtk_widget_set_size_request(search_hbox, 850, 25);

    GtkWidget* scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(scrolled_window, 850, 475);

    gtk_box_pack_start(GTK_BOX(content_vbox), dir_hbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(content_vbox), search_hbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(content_vbox), list_header_label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(content_vbox), scrolled_window, TRUE, TRUE, 0);

    // dir_box_h
    // dir_text = gtk_label_new(cwd);
    // //  gtk_widget_set_name(dir_text, "dir_text");
    //  gtk_style_class_toggle(dir_text, "dir_text", TRUE);
    // gtk_box_pack_start(GTK_BOX(dir_hbox), dir_text, FALSE, FALSE, 0);

    int* index;
    for(int i=0;i<MAX_PATH_TOKEN;++i){
        GtkWidget* event_box = gtk_event_box_new();
        gtk_style_class_toggle(event_box, "path_tok", TRUE);
        // gtk_widget_set_name(event_box, "file_info_text");
        //  gtk_style_class_toggle(event_box, "file_item_box", TRUE);

        path_tok_label[i] = gtk_label_new("");
        // gtk_style_class_toggle(path_tok_label[i], "file_item_text", TRUE);
        gtk_label_set_xalign(GTK_LABEL(path_tok_label[i]), 0.0);
        gtk_container_add(GTK_CONTAINER(event_box), path_tok_label[i]);

        index = g_new(int, 1);
        *index = i;
        g_object_set_data(G_OBJECT(event_box), "index", index);

        g_signal_connect(event_box, "button-press-event", G_CALLBACK(g_callback_mvdir_tok), NULL);
        g_signal_connect(event_box, "enter-notify-event", G_CALLBACK(on_enter_notify_path_tok), NULL);
        g_signal_connect(event_box, "leave-notify-event", G_CALLBACK(on_leave_notify_path_tok), NULL);

        gtk_box_pack_start(GTK_BOX(dir_hbox), event_box, FALSE, FALSE, 10);
    }

    // search_hbox
    search_inp = gtk_entry_new();
    gtk_style_class_toggle(search_inp, "inp_search", TRUE);
     gtk_widget_set_size_request(search_inp, 500, 25);
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_inp), "search by filename...");
    GtkWidget* btn_search;
    btn_search = gtk_button_new_with_label("Search");
     gtk_widget_set_size_request(btn_search, 100, 25);
    gtk_style_class_toggle(btn_search, "btn_search", TRUE);

    gtk_box_pack_start(GTK_BOX(search_hbox), search_inp, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(search_hbox), btn_search, TRUE, TRUE, 0);

     g_signal_connect(search_inp, "changed", G_CALLBACK(g_callback_search_inp_changed), NULL);
     g_signal_connect(btn_search, "clicked", G_CALLBACK(g_callback_search_file), NULL);

    // list_box
    list_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    // gtk_widget_set_name(list_vbox, "list_box");
     gtk_style_class_toggle(list_vbox, "list_box", TRUE);

    gtk_container_add(GTK_CONTAINER(scrolled_window), list_vbox);
    // int* index;
    printf("label build..\n");
    for(int i=0;i<MAX_FILE_LIST_SIZE;++i){
        GtkWidget* event_box = gtk_event_box_new();
        // gtk_widget_set_name(event_box, "file_info_text");
         gtk_style_class_toggle(event_box, "file_item", TRUE);

        label_data[i] = gtk_label_new("test label");
        gtk_style_class_toggle(label_data[i], "file_info_text", TRUE);
        gtk_label_set_xalign(GTK_LABEL(label_data[i]), 0.0);
        gtk_container_add(GTK_CONTAINER(event_box), label_data[i]);

        index = g_new(int, 1);
        *index = i;
        g_object_set_data(G_OBJECT(event_box), "index", index);

        g_signal_connect(event_box, "button-press-event", G_CALLBACK(g_callback_item_clicked), NULL);
        g_signal_connect(event_box, "enter-notify-event", G_CALLBACK(on_enter_notify), NULL);
        g_signal_connect(event_box, "leave-notify-event", G_CALLBACK(on_leave_notify), NULL);

        gtk_box_pack_start(GTK_BOX(list_vbox), event_box, FALSE, FALSE, 0);
    }
    
    // footer_h :
    GtkWidget* clipboard_hbox, *clip_title;
    clipboard_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    // gtk_widget_set_name(clipboard_vbox, "clipboard_box");
     gtk_style_class_toggle(clipboard_hbox, "clipboard_cont", TRUE);
    gtk_box_pack_start(GTK_BOX(footer_vbox), clipboard_hbox, TRUE, TRUE, 0);

    clip_title = gtk_label_new("Clipboard");
    gtk_widget_set_size_request(clip_title, 200, 30);
    gtk_style_class_toggle(clip_title, "clip_title", TRUE);
    gtk_box_pack_start(GTK_BOX(clipboard_hbox), clip_title, TRUE, TRUE, 0);

    copied_file_label = gtk_label_new("");
    gtk_widget_set_size_request(copied_file_label, 750, 30);
    gtk_style_class_toggle(copied_file_label, "clipboard_text", TRUE);
    gtk_box_pack_start(GTK_BOX(clipboard_hbox), copied_file_label, TRUE, TRUE, 0);

    GtkWidget* btn_clear_clipboard;
    btn_clear_clipboard = gtk_button_new_with_label("Clear");
    gtk_widget_set_size_request(btn_clear_clipboard, 50, 30);
    gtk_style_class_toggle(btn_clear_clipboard, "btn_clear_clipboard", TRUE);
     g_signal_connect(btn_clear_clipboard, "clicked", G_CALLBACK(g_callback_clear_clipboard), NULL);


    gtk_box_pack_start(GTK_BOX(clipboard_hbox), btn_clear_clipboard, TRUE, TRUE, 0);
    
    dialog_label = gtk_label_new("");
    // gtk_widget_set_name(dialog_label, "dialog_label");
     gtk_style_class_toggle(dialog_label, "msg_text", TRUE);
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

gboolean timeout_inform(){
    gtk_label_set_text(dialog_label, "");
    gtk_style_class_toggle(dialog_label, CLASS_INFORM, FALSE);
    return FALSE;
}

void inform_dialog(char* msg){
    gtk_label_set_text(dialog_label, msg);
    gtk_style_class_toggle(dialog_label, CLASS_INFORM, TRUE);
    g_timeout_add(3000, timeout_inform, NULL);
}

gboolean timeout_error(){
    gtk_label_set_text(dialog_label, "");
    gtk_style_class_toggle(dialog_label, CLASS_ERROR, FALSE);
    return FALSE;
}

void error_dialog(char* msg){
    gtk_label_set_text(dialog_label, msg);
    gtk_style_class_toggle(dialog_label, CLASS_ERROR, TRUE);
    g_timeout_add(3000, timeout_error, NULL);
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