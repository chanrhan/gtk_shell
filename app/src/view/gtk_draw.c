#include "gtk_draw.h"


void update_directory(res_msg_t res){
    for(int i=0;i<res.data.file_len;++i){
        file_list[i] = res.data.files[i];
    }

    char* text;
    int i=0;

    while(i < res.data.file_len){
        // printf("file[%d]: %s %d %s %d\n", i, file_list[i].name, file_list[i].type, file_list[i].atime, file_list[i].size);
        text = (char*)malloc(32);
        snprintf(text, 32, "%s\t\t\t\t\t\t\t %d\t %s\t %d", file_list[i].name, file_list[i].type, file_list[i].atime, file_list[i].size);
        gtk_label_set_text(GTK_LABEL(label_data[i]), text);
        ++i;
    }
    while(i < FILE_DISPLAY_LIMIT){
        gtk_label_set_text(GTK_LABEL(label_data[i]), "");
        ++i;
    }
}

void set_text_current_dir(char* new_cwd){
    strncpy(cwd, new_cwd, CWD_LEN);
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

void mkdir_popup_init(GtkWidget* parent){
    int MAX_WIDTH = 600;
    int MAX_HEIGHT = 400;
    // add_btn
    mkdir_modal.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(mkdir_modal.window), "Popup Window");
    gtk_window_set_position(GTK_WINDOW(mkdir_modal.window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(mkdir_modal.window), MAX_WIDTH, MAX_HEIGHT);
    
    gtk_window_set_transient_for(GTK_WINDOW(mkdir_modal.window), GTK_WINDOW(parent));
    gtk_window_set_modal(GTK_WINDOW(mkdir_modal.window), TRUE);

    // layout
    GtkWidget* vbox, *func_vbox, *ctrl_hbox;
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(mkdir_modal.window), vbox);

    func_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(func_vbox, MAX_WIDTH, 300);

    ctrl_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(ctrl_hbox, MAX_WIDTH, 100);

    gtk_box_pack_start(GTK_BOX(vbox), func_vbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), ctrl_hbox, TRUE, TRUE, 0);

    // func_box
    GtkWidget* setting_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(setting_hbox, 600, 75);
    gtk_box_pack_start(GTK_BOX(func_vbox), setting_hbox, TRUE, TRUE, 0);
    GtkWidget* perm_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(perm_vbox, 600, 225);
    gtk_box_pack_start(GTK_BOX(func_vbox), perm_vbox, TRUE, TRUE, 0);

    // setting_hbox
    GtkWidget* type_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(setting_hbox), type_vbox, TRUE, TRUE, 0);
    GtkWidget* inp_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(setting_hbox), inp_vbox, TRUE, TRUE, 0);

    // type_vbox
    GtkWidget* type_label = gtk_label_new("file type");
    gtk_box_pack_start(GTK_BOX(type_vbox), type_label, TRUE, TRUE, 0);

    mkdir_modal.select_type = gtk_combo_box_text_new();
    gtk_widget_set_size_request(mkdir_modal.select_type, 100, 100);

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(mkdir_modal.select_type), "directory");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(mkdir_modal.select_type), "file");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(mkdir_modal.select_type), "link");

    gtk_combo_box_set_active(GTK_COMBO_BOX(mkdir_modal.select_type), 0);  // 기본 선택 항목 설정

    gtk_box_pack_start(GTK_BOX(type_vbox), mkdir_modal.select_type, TRUE, TRUE, 0);

    // inp_vbox
    GtkWidget* inp_label = gtk_label_new("file name");
    gtk_box_pack_start(GTK_BOX(inp_vbox), inp_label, TRUE, TRUE, 0);
    mkdir_modal.inp_filename = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(mkdir_modal.inp_filename), "Enter some text here...");
    gtk_box_pack_start(GTK_BOX(inp_vbox), mkdir_modal.inp_filename, FALSE, FALSE, 0);

    mkdir_modal.inp_error_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(inp_vbox), mkdir_modal.inp_error_label, TRUE, TRUE, 0);
    
    // perm_hbox
    GtkWidget* perm_label =  gtk_label_new("file access permission");
    gtk_box_pack_start(GTK_BOX(perm_vbox), perm_label, TRUE, TRUE, 0);

    GtkWidget* ugo_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(perm_vbox), ugo_hbox, TRUE, TRUE, 0);
    GtkWidget* u_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget* g_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget* o_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(ugo_hbox), u_vbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(ugo_hbox), g_vbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(ugo_hbox), o_vbox, TRUE, TRUE, 0);

    GtkWidget* u_label =  gtk_label_new("user");
    gtk_box_pack_start(GTK_BOX(u_vbox), u_label, TRUE, TRUE, 0);
    mkdir_modal.perm_check_box[8] = gtk_check_button_new_with_label("read");
    mkdir_modal.perm_check_box[7] = gtk_check_button_new_with_label("write");
    mkdir_modal.perm_check_box[6] = gtk_check_button_new_with_label("execute");
    gtk_box_pack_start(GTK_BOX(u_vbox), mkdir_modal.perm_check_box[8], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(u_vbox), mkdir_modal.perm_check_box[7], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(u_vbox), mkdir_modal.perm_check_box[6], TRUE, TRUE, 0);

    GtkWidget* g_label =  gtk_label_new("group");
    gtk_box_pack_start(GTK_BOX(g_vbox), g_label, TRUE, TRUE, 0);
    mkdir_modal.perm_check_box[5] = gtk_check_button_new_with_label("read");
    mkdir_modal.perm_check_box[4] = gtk_check_button_new_with_label("write");
    mkdir_modal.perm_check_box[3] = gtk_check_button_new_with_label("execute");
    gtk_box_pack_start(GTK_BOX(g_vbox), mkdir_modal.perm_check_box[5], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(g_vbox), mkdir_modal.perm_check_box[4], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(g_vbox), mkdir_modal.perm_check_box[3], TRUE, TRUE, 0);

    GtkWidget* o_label =  gtk_label_new("others");
    gtk_box_pack_start(GTK_BOX(o_vbox), o_label, TRUE, TRUE, 0);
    mkdir_modal.perm_check_box[2] = gtk_check_button_new_with_label("read");
    mkdir_modal.perm_check_box[1] = gtk_check_button_new_with_label("write");
    mkdir_modal.perm_check_box[0] = gtk_check_button_new_with_label("execute");
    gtk_box_pack_start(GTK_BOX(o_vbox), mkdir_modal.perm_check_box[2], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(o_vbox), mkdir_modal.perm_check_box[1], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(o_vbox), mkdir_modal.perm_check_box[0], TRUE, TRUE, 0);

    // ctrl_box
    mkdir_modal.btn_close = gtk_button_new_with_label("취소");
    mkdir_modal.btn_submit = gtk_button_new_with_label("생성");
    g_signal_connect(mkdir_modal.btn_close, "clicked", G_CALLBACK(on_popup_close), mkdir_modal.window);
    g_signal_connect(mkdir_modal.btn_submit, "clicked", G_CALLBACK(g_callback_mkdir_popup_submit), NULL);

    gtk_box_pack_start(GTK_BOX(ctrl_hbox), mkdir_modal.btn_submit, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(ctrl_hbox), mkdir_modal.btn_close, TRUE, TRUE, 5);
}


void build_layout(GtkWidget* window){
    GtkWidget *vbox;

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // vbox :
    GtkWidget *header_h, *body_h, *footer_h;
    header_h = gtk_hbox_new(FALSE, 0);
    gtk_widget_set_size_request(header_h, 1000, 50);
    gtk_widget_set_name(header_h, "header_box");

    body_h = gtk_hbox_new(FALSE, 0);
    gtk_widget_set_size_request(body_h, 1000, 650);
    gtk_widget_set_name(body_h, "body_box");

    footer_h = gtk_hbox_new(FALSE, 0);
    gtk_widget_set_size_request(footer_h, 1000, 100);
    gtk_widget_set_name(footer_h, "footer_box");

    gtk_box_pack_start(GTK_BOX(vbox), header_h, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), body_h, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), footer_h, TRUE, TRUE, 0);

    // header_h :
    GtkWidget *btn_mkdir, *btn_ps, *btn_quit;

    btn_mkdir = gtk_button_new_with_label("create file");
    gtk_widget_set_name(btn_mkdir, "custom_button");
    btn_ps = gtk_button_new_with_label("show process");
    btn_quit = gtk_button_new_with_label("quit");

    g_signal_connect(btn_mkdir, "clicked", G_CALLBACK(g_callback_mkdir_popup_open), NULL);

    g_signal_connect(btn_quit, "clicked", G_CALLBACK(g_callback_quit), NULL);

    gtk_box_pack_start(GTK_BOX(header_h), btn_mkdir, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(header_h), btn_ps, FALSE, FALSE, 5);
    gtk_box_pack_end(GTK_BOX(header_h), btn_quit, FALSE, FALSE, 5);

    mkdir_popup_init(window);

    // body_h :
    GtkWidget* side_box_v, *content_box_v;
    side_box_v = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_size_request(side_box_v, 150, 650);
    gtk_widget_set_name(side_box_v, "side_box");

    content_box_v = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_size_request(content_box_v, 850, 650);
    gtk_widget_set_name(content_box_v, "content_box");

    gtk_box_pack_start(GTK_BOX(body_h), side_box_v, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(body_h), content_box_v, TRUE, TRUE, 5);

    // side_box_v :

    // content_box_v :
    GtkWidget* dir_box_h, *search_box_h, *list_box;
    dir_box_h = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_size_request(dir_box_h, 850, 50);
    gtk_widget_set_name(dir_box_h, "dir_box");

    search_box_h = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_size_request(search_box_h, 850, 50);
    gtk_widget_set_name(search_box_h, "search_box");

    list_box = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_size_request(list_box, 850, 550);
    gtk_widget_set_name(list_box, "list_box");

    gtk_box_pack_start(GTK_BOX(content_box_v), dir_box_h, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(content_box_v), search_box_h, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(content_box_v), list_box, TRUE, TRUE, 0);

    // dir_box_h
    dir_text = gtk_label_new(cwd);
     gtk_widget_set_name(dir_text, "dir_text");
    gtk_box_pack_start(GTK_BOX(dir_box_h), dir_text, FALSE, FALSE, 0);

    // list_box
    int* index;
    for(int i=0;i<FILE_DISPLAY_LIMIT;++i){
        GtkWidget* event_box = gtk_event_box_new();
        gtk_widget_set_name(event_box, "file_info_text");

        label_data[i] = gtk_label_new("test label");
        gtk_label_set_xalign(GTK_LABEL(label_data[i]), 0.0);
        gtk_container_add(GTK_CONTAINER(event_box), label_data[i]);

        index = g_new(int, 1);
        *index = i;
        g_object_set_data(G_OBJECT(event_box), "index", index);

        g_signal_connect(event_box, "button-press-event", G_CALLBACK(g_callback_mvdir), NULL);
        g_signal_connect(event_box, "enter-notify-event", G_CALLBACK(on_enter_notify), NULL);
        g_signal_connect(event_box, "leave-notify-event", G_CALLBACK(on_leave_notify), NULL);

        gtk_box_pack_start(GTK_BOX(list_box), event_box, FALSE, FALSE, 10);
    }
    
    // footer_h :
    GtkWidget* clipboard_box;
    clipboard_box = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_name(clipboard_box, "clipboard_box");
    gtk_box_pack_start(GTK_BOX(footer_h), clipboard_box, TRUE, TRUE, 0);
}