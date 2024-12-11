#include "gtk_modal.h"
#include "gtk_callback.h"


void init_file_menu_context(GtkWidget* parent){
    int MAX_WIDTH = 600;
    int MAX_HEIGHT = 400;
    // add_btn
    GtkWidget* menu, *item_cp, *item_rm, *item_rename, *item_mv, *item_md, *item_ex, *item_mklink;
    menu = gtk_menu_new();
    
    item_cp = gtk_menu_item_new_with_label("Copy");
    item_rm= gtk_menu_item_new_with_label("Remove");
    item_rename = gtk_menu_item_new_with_label("Rename");
    item_mv = gtk_menu_item_new_with_label("Move");
    item_md = gtk_menu_item_new_with_label("Modify");
    item_mklink = gtk_menu_item_new_with_label("Link");
    item_ex = gtk_menu_item_new_with_label("Execute");

    g_signal_connect(item_cp, "activate", G_CALLBACK(g_callback_file_copy), NULL);
    g_signal_connect(item_rm, "activate", G_CALLBACK(g_callback_file_remove), NULL);
    g_signal_connect(item_rename, "activate", G_CALLBACK(g_callback_file_rename), NULL);
    g_signal_connect(item_mv, "activate", G_CALLBACK(g_callback_file_move), NULL);
    g_signal_connect(item_md, "activate", G_CALLBACK(g_callback_file_modify), NULL);
    g_signal_connect(item_ex, "activate", G_CALLBACK(g_callback_file_execute), NULL);
    g_signal_connect(item_mklink, "activate", G_CALLBACK(g_callback_file_mklink), NULL);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_cp);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_rm);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_rename);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_mv);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_ex);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_md);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_mklink);

    gtk_widget_show_all(menu);

    file_menu_context.menu = menu;
    file_menu_context.item_copy = item_cp;
    file_menu_context.item_move = item_mv;
    file_menu_context.item_rename = item_rename;
    file_menu_context.item_remove = item_rm;
    file_menu_context.item_modify = item_md;
    file_menu_context.item_execute = item_ex;
    file_menu_context.item_mklink = item_mklink;
}

void init_dir_menu_context(GtkWidget* parent){
    int MAX_WIDTH = 600;
    int MAX_HEIGHT = 400;
    // add_btn
    GtkWidget* menu, *item_cp, *item_rm, *item_rename, *item_mv;
    menu = gtk_menu_new();

    item_cp = gtk_menu_item_new_with_label("Copy");
    item_rm= gtk_menu_item_new_with_label("Remove");
    item_rename = gtk_menu_item_new_with_label("Rename");
    item_mv = gtk_menu_item_new_with_label("Move");

    g_signal_connect(item_cp, "activate", G_CALLBACK(g_callback_file_copy), NULL);
    g_signal_connect(item_rm, "activate", G_CALLBACK(g_callback_dir_remove), NULL);
    g_signal_connect(item_rename, "activate", G_CALLBACK(g_callback_file_rename), NULL);
    g_signal_connect(item_mv, "activate", G_CALLBACK(g_callback_file_move), NULL);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_cp);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_rm);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_rename);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_mv);

    gtk_widget_show_all(menu);

    dir_menu_context.menu = menu;
    dir_menu_context.item_copy = item_cp;
    dir_menu_context.item_move = item_mv;
    dir_menu_context.item_rename = item_rename;
    dir_menu_context.item_remove = item_rm;
}


void init_mkdir_modal(GtkWidget* parent){
    int MAX_WIDTH = 600;
    int MAX_HEIGHT = 400;
    // add_btn
    mkdir_modal.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(mkdir_modal.window), "File Info");
    gtk_window_set_position(GTK_WINDOW(mkdir_modal.window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(mkdir_modal.window), MAX_WIDTH, MAX_HEIGHT);
    
    gtk_window_set_transient_for(GTK_WINDOW(mkdir_modal.window), GTK_WINDOW(parent));
    gtk_window_set_modal(GTK_WINDOW(mkdir_modal.window), TRUE);

    // layout (600, 400)
    GtkWidget* vbox, *func_vbox, *ctrl_hbox;
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(mkdir_modal.window), vbox);

    func_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(func_vbox, MAX_WIDTH, 350);

    ctrl_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(ctrl_hbox, MAX_WIDTH, 50);

    gtk_box_pack_start(GTK_BOX(vbox), func_vbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), ctrl_hbox, TRUE, TRUE, 0);

    // func_box (600, 350)
    GtkWidget* setting_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(setting_hbox, 600, 45);
    gtk_box_pack_start(GTK_BOX(func_vbox), setting_hbox, TRUE, TRUE, 0);
    GtkWidget* perm_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(perm_vbox, 600, 255);
    gtk_box_pack_start(GTK_BOX(func_vbox), perm_vbox, TRUE, TRUE, 0);

    // setting_hbox (600, 45)
    GtkWidget* type_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(type_vbox, 200, 45);
    gtk_box_pack_start(GTK_BOX(setting_hbox), type_vbox, TRUE, TRUE, 0);
    GtkWidget* inp_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(inp_vbox, 400, 45);
    gtk_box_pack_start(GTK_BOX(setting_hbox), inp_vbox, TRUE, TRUE, 0);

    // type_vbox (200, 45)
    // GtkWidget* type_label = gtk_label_new("File Type");
    // gtk_widget_set_size_request(type_label, 200, 20);
    // gtk_box_pack_start(GTK_BOX(type_vbox), type_label, FALSE, FALSE, 0);

    // mkdir_modal.select_type = gtk_combo_box_text_new();
    // gtk_widget_set_size_request(mkdir_modal.select_type, 200, 25);

    // gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(mkdir_modal.select_type), "directory");
    // gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(mkdir_modal.select_type), "file");
    // gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(mkdir_modal.select_type), "link");

    // gtk_combo_box_set_active(GTK_COMBO_BOX(mkdir_modal.select_type), 0);  // 기본 선택 항목 설정

    // gtk_box_pack_start(GTK_BOX(type_vbox), mkdir_modal.select_type, FALSE, FALSE, 0);

    // inp_vbox (400, 45)
    GtkWidget* inp_label = gtk_label_new("File Name");
    gtk_widget_set_size_request(inp_label, 400, 20);
    gtk_box_pack_start(GTK_BOX(inp_vbox), inp_label, FALSE, FALSE, 0);
    mkdir_modal.inp_filename = gtk_entry_new();
    gtk_widget_set_size_request(mkdir_modal.inp_filename, 400, 25);
    gtk_entry_set_placeholder_text(GTK_ENTRY(mkdir_modal.inp_filename), "input new filename...");
    gtk_box_pack_start(GTK_BOX(inp_vbox), mkdir_modal.inp_filename, FALSE, FALSE, 0);

    mkdir_modal.inp_error_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(inp_vbox), mkdir_modal.inp_error_label, FALSE, FALSE, 0);
    
    // perm_hbox (600, 225)
    GtkWidget* perm_label =  gtk_label_new("File Access Permission");
    gtk_widget_set_size_request(perm_label, 600, 25);
    gtk_box_pack_start(GTK_BOX(perm_vbox), perm_label, FALSE, FALSE, 0);

    GtkWidget* ugo_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(ugo_hbox, 600, 200);
    gtk_box_pack_start(GTK_BOX(perm_vbox), ugo_hbox, FALSE, FALSE, 0);
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
    gtk_box_pack_start(GTK_BOX(o_vbox), o_label, TRUE, TRUE, 10);
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

void init_modal(GtkWidget* parent){
    init_mkdir_modal(parent);
    init_file_menu_context(parent);
    init_dir_menu_context(parent);
}
