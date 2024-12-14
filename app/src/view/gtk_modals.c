#include "gtk_modal.h"
#include "gtk_callback.h"
#include "gtk_draw.h"

void init_bg_menu_context(GtkWidget* parent){
    int MAX_WIDTH = 600;
    int MAX_HEIGHT = 400;
    // add_btn
    GtkWidget* menu, *item_paste, *item_open_terminal, *item_create_dir, *item_create_file;
    menu = gtk_menu_new();
    
    item_paste= gtk_menu_item_new_with_label("Paste");
    item_create_dir = gtk_menu_item_new_with_label("Create Directory");
    item_create_file = gtk_menu_item_new_with_label("Create File");
    item_open_terminal = gtk_menu_item_new_with_label("Open Terminal");

    g_signal_connect(item_paste, "activate", G_CALLBACK(g_callback_file_paste), NULL);
    g_signal_connect(item_create_dir, "activate", G_CALLBACK(g_callback_mkdir_popup_open), NULL);
    g_signal_connect(item_create_file, "activate", G_CALLBACK(g_callback_file_info_open), NULL);
    // g_signal_connect(item_open_terminal, "activate", G_CALLBACK(g_callback_file_rename), NULL);
    // g_signal_connect(item_create, "activate", G_CALLBACK(g_callback_file_move), NULL);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_paste);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_create_dir);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_create_file);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_open_terminal);

    gtk_widget_show_all(menu);

    ctxm_bg_option.menu = menu;
    ctxm_bg_option.item_paste = item_paste;
    ctxm_bg_option.item_create_dir = item_create_dir;
    ctxm_bg_option.item_create_file = item_create_dir;
    ctxm_bg_option.item_open_terminal = item_open_terminal;
}

void init_file_menu_context(GtkWidget* parent){
    int MAX_WIDTH = 600;
    int MAX_HEIGHT = 400;
    // add_btn
    GtkWidget* menu, *item_cp, *item_detail, *item_rm, *item_rename, *item_mv, *item_md, *item_ex, *item_mklink;
    menu = gtk_menu_new();
    
    item_cp = gtk_menu_item_new_with_label("Copy");
    item_detail = gtk_menu_item_new_with_label("Detail..");
    item_rm= gtk_menu_item_new_with_label("Remove");
    item_rename = gtk_menu_item_new_with_label("Rename");
    item_mv = gtk_menu_item_new_with_label("Cut");
    item_md = gtk_menu_item_new_with_label("Modify");
    item_mklink = gtk_menu_item_new_with_label("Link");
    item_ex = gtk_menu_item_new_with_label("Execute");

    g_signal_connect(item_cp, "activate", G_CALLBACK(g_callback_file_copy), NULL);
    g_signal_connect(item_detail, "activate", G_CALLBACK(g_callback_open_file_detail), NULL);
    g_signal_connect(item_rm, "activate", G_CALLBACK(g_callback_file_remove), NULL);
    g_signal_connect(item_rename, "activate", G_CALLBACK(g_callback_file_rename), NULL);
    g_signal_connect(item_mv, "activate", G_CALLBACK(g_callback_file_move), NULL);
    g_signal_connect(item_md, "activate", G_CALLBACK(g_callback_file_modify), NULL);
    g_signal_connect(item_ex, "activate", G_CALLBACK(g_callback_file_execute), NULL);
    g_signal_connect(item_mklink, "activate", G_CALLBACK(g_callback_file_mklink), NULL);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_cp);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_detail);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_mv);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_rename);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_md);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_mklink);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_ex);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_rm);

    gtk_widget_show_all(menu);

    ctxm_file_option.menu = menu;
    ctxm_file_option.item_copy = item_cp;
    ctxm_file_option.item_detail = item_detail;
    ctxm_file_option.item_move = item_mv;
    ctxm_file_option.item_rename = item_rename;
    ctxm_file_option.item_remove = item_rm;
    ctxm_file_option.item_modify = item_md;
    ctxm_file_option.item_execute = item_ex;
    ctxm_file_option.item_mklink = item_mklink;
}

void init_dir_menu_context(GtkWidget* parent){
    int MAX_WIDTH = 600;
    int MAX_HEIGHT = 400;
    // add_btn
    GtkWidget* menu, *item_cp, *item_detail, *item_rm, *item_rename, *item_mv;
    menu = gtk_menu_new();

    item_cp = gtk_menu_item_new_with_label("Copy");
    item_detail = gtk_menu_item_new_with_label("Detail..");
    item_rm= gtk_menu_item_new_with_label("Remove");
    item_rename = gtk_menu_item_new_with_label("Rename");
    item_mv = gtk_menu_item_new_with_label("Move");

    g_signal_connect(item_cp, "activate", G_CALLBACK(g_callback_file_copy), NULL);
    g_signal_connect(item_detail, "activate", G_CALLBACK(g_callback_open_file_detail), NULL);
    g_signal_connect(item_rm, "activate", G_CALLBACK(g_callback_dir_remove), NULL);
    g_signal_connect(item_rename, "activate", G_CALLBACK(g_callback_file_rename), NULL);
    g_signal_connect(item_mv, "activate", G_CALLBACK(g_callback_file_move), NULL);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_cp);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_detail);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_rm);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_rename);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item_mv);

    gtk_widget_show_all(menu);

    ctxm_dir_option.menu = menu;
    ctxm_dir_option.item_copy = item_cp;
    ctxm_dir_option.item_detail = item_detail;
    ctxm_dir_option.item_move = item_mv;
    ctxm_dir_option.item_rename = item_rename;
    ctxm_dir_option.item_remove = item_rm;
}


void init_mkdir_modal(GtkWidget* parent){
    int MAX_WIDTH = 250;
    int MAX_HEIGHT = 100;
    // add_btn
    md_mkdir.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(md_mkdir.window), "File Info");
    gtk_window_set_position(GTK_WINDOW(md_mkdir.window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(md_mkdir.window), MAX_WIDTH, MAX_HEIGHT);
    
    gtk_window_set_transient_for(GTK_WINDOW(md_mkdir.window), GTK_WINDOW(parent));
    gtk_window_set_modal(GTK_WINDOW(md_mkdir.window), TRUE);

    // layout (600, 400)
    GtkWidget* vbox, *func_vbox, *ctrl_hbox;
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(md_mkdir.window), vbox);

    func_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(func_vbox, MAX_WIDTH, 75);

    ctrl_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(ctrl_hbox, MAX_WIDTH, 25);

    gtk_box_pack_start(GTK_BOX(vbox), func_vbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), ctrl_hbox, TRUE, TRUE, 0);

    // func_box (600, 350)
    // GtkWidget* setting_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    // gtk_widget_set_size_request(setting_hbox, MAX_WIDTH, 150);
    // gtk_box_pack_start(GTK_BOX(func_vbox), setting_hbox, TRUE, TRUE, 0);
    // GtkWidget* perm_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    // gtk_widget_set_size_request(perm_vbox, MAX_WIDTH, 255);
    // gtk_box_pack_start(GTK_BOX(func_vbox), perm_vbox, TRUE, TRUE, 0);

    // setting_hbox (600, 45)
    // GtkWidget* type_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    // gtk_widget_set_size_request(type_vbox, 200, 45);
    // gtk_box_pack_start(GTK_BOX(setting_hbox), type_vbox, TRUE, TRUE, 0);
    GtkWidget* inp_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(inp_vbox, MAX_WIDTH, 75);
    gtk_box_pack_start(GTK_BOX(func_vbox), inp_vbox, TRUE, TRUE, 0);

    // inp_vbox (400, 45)
    GtkWidget* inp_label = gtk_label_new("File Name");
    gtk_widget_set_size_request(inp_label, MAX_WIDTH, 20);
    gtk_box_pack_start(GTK_BOX(inp_vbox), inp_label, FALSE, FALSE, 0);
    md_mkdir.inp_filename = gtk_entry_new();
    gtk_widget_set_size_request(md_mkdir.inp_filename, MAX_WIDTH-5, 25);
    gtk_entry_set_placeholder_text(GTK_ENTRY(md_mkdir.inp_filename), "input new filename...");
    gtk_box_pack_start(GTK_BOX(inp_vbox), md_mkdir.inp_filename, FALSE, FALSE, 0);

    md_mkdir.inp_error_label = gtk_label_new("");
    gtk_widget_set_size_request(md_mkdir.inp_error_label, MAX_WIDTH, 25);
    gtk_box_pack_start(GTK_BOX(inp_vbox), md_mkdir.inp_error_label, FALSE, FALSE, 0);

    // ctrl_box
    md_mkdir.btn_close = gtk_button_new_with_label("Cancel");
    md_mkdir.btn_submit = gtk_button_new_with_label("Save");
    g_signal_connect(md_mkdir.btn_close, "clicked", G_CALLBACK(on_popup_close), md_mkdir.window);
    g_signal_connect(md_mkdir.btn_submit, "clicked", G_CALLBACK(g_callback_mkdir_popup_submit), NULL);

    gtk_box_pack_start(GTK_BOX(ctrl_hbox), md_mkdir.btn_submit, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(ctrl_hbox), md_mkdir.btn_close, TRUE, TRUE, 0);
}

void init_modal_file_detail(GtkWidget* parent){
    int MAX_WIDTH = 250;
    int MAX_HEIGHT = 400;
    // add_btn
    md_file_detail.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(md_file_detail.window), "File Info");
    gtk_window_set_position(GTK_WINDOW(md_file_detail.window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(md_file_detail.window), MAX_WIDTH, MAX_HEIGHT);
    
    gtk_window_set_transient_for(GTK_WINDOW(md_file_detail.window), GTK_WINDOW(parent));
    gtk_window_set_modal(GTK_WINDOW(md_file_detail.window), TRUE);

    // layout (600, 400)
    GtkWidget* vbox, *main_vbox, *ctrl_hbox;
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(md_file_detail.window), vbox);
    gtk_style_class_toggle(vbox, "file_detail_cont", TRUE);
    

    main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(main_vbox, MAX_WIDTH, 350);

    ctrl_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(ctrl_hbox, MAX_WIDTH, 50);

    gtk_box_pack_start(GTK_BOX(vbox), main_vbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), ctrl_hbox, TRUE, TRUE, 0);

    // info_vbox (600, 350)
    GtkWidget* info_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(info_vbox, MAX_WIDTH, 45);
    gtk_style_class_toggle(info_vbox, "file_info_box", TRUE);

    gtk_box_pack_start(GTK_BOX(main_vbox), info_vbox, TRUE, TRUE, 0);
    GtkWidget* perm_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_style_class_toggle(perm_vbox, "perm_cont", TRUE);

    gtk_widget_set_size_request(perm_vbox, MAX_WIDTH, 255);
    gtk_box_pack_start(GTK_BOX(main_vbox), perm_vbox, TRUE, TRUE, 5);

    GtkWidget* filename_field,*mtime_field,*btime_field,*size_field;
    filename_field = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_style_class_toggle(filename_field, "text_field", TRUE);

    mtime_field = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_style_class_toggle(mtime_field, "text_field", TRUE);

    btime_field = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_style_class_toggle(btime_field, "text_field", TRUE);

    size_field = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_style_class_toggle(size_field, "text_field", TRUE);

    gtk_label_set_xalign(GTK_LABEL(filename_field), 0.0);
    gtk_label_set_xalign(GTK_LABEL(mtime_field), 0.0);
    gtk_label_set_xalign(GTK_LABEL(btime_field), 0.0);
    gtk_label_set_xalign(GTK_LABEL(size_field), 0.0);

    gtk_box_pack_start(GTK_BOX(info_vbox), filename_field, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(info_vbox), mtime_field, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(info_vbox), btime_field, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(info_vbox), size_field, FALSE, FALSE, 0);

    GtkWidget* lb1, *lb2,*lb3,*lb4;

    lb1 = gtk_label_new("Filename:  ");
    md_file_detail.filename_label = gtk_label_new("");
    // gtk_label_set_xalign(GTK_LABEL(md_file_detail.filename_label), 0.0);
    gtk_box_pack_start(GTK_BOX(filename_field), lb1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(filename_field), md_file_detail.filename_label, FALSE, FALSE, 0);

    lb2 = gtk_label_new("Modifed Time:  ");
    md_file_detail.mtime_label = gtk_label_new("");
    // gtk_label_set_xalign(GTK_LABEL(md_file_detail.mtime_label), 0.0);
    gtk_box_pack_start(GTK_BOX(mtime_field), lb2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mtime_field), md_file_detail.mtime_label, FALSE, FALSE, 0);

    lb3 = gtk_label_new("Create Time:  ");
    md_file_detail.birthtimetime_label = gtk_label_new("");
    // gtk_label_set_xalign(GTK_LABEL(md_file_detail.birthtimetime_label), 0.0);
    gtk_box_pack_start(GTK_BOX(btime_field), lb3, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(btime_field), md_file_detail.birthtimetime_label, FALSE, FALSE, 0);

    lb4 = gtk_label_new("Size:  ");
    md_file_detail.size_label = gtk_label_new("");
    // gtk_label_set_xalign(GTK_LABEL(md_file_detail.size_label), 0.0);
    gtk_box_pack_start(GTK_BOX(size_field), lb4, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(size_field), md_file_detail.size_label, FALSE, FALSE, 0);
    
    
    // perm_hbox (600, 225)
    GtkWidget* perm_label =  gtk_label_new("File Access Permission");

    gtk_widget_set_size_request(perm_label, MAX_WIDTH, 25);
    gtk_box_pack_start(GTK_BOX(perm_vbox), perm_label, FALSE, FALSE, 0);

    GtkWidget* ugo_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_style_class_toggle(ugo_vbox, "ugo_cont", TRUE);
    
    gtk_widget_set_size_request(ugo_vbox, MAX_WIDTH, 200);
    gtk_box_pack_start(GTK_BOX(perm_vbox), ugo_vbox, FALSE, FALSE, 0);
    GtkWidget* u_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget* g_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget* o_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(ugo_vbox), u_vbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(ugo_vbox), g_vbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(ugo_vbox), o_vbox, TRUE, TRUE, 0);

    GtkWidget* u_label =  gtk_label_new("user");
    gtk_label_set_xalign(GTK_LABEL(u_label), 0.0);

    gtk_box_pack_start(GTK_BOX(u_vbox), u_label, TRUE, TRUE, 0);
    md_file_detail.perm_check_box[8] = gtk_check_button_new_with_label("read");
    md_file_detail.perm_check_box[7] = gtk_check_button_new_with_label("write");
    md_file_detail.perm_check_box[6] = gtk_check_button_new_with_label("execute");
    gtk_box_pack_start(GTK_BOX(u_vbox), md_file_detail.perm_check_box[8], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(u_vbox), md_file_detail.perm_check_box[7], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(u_vbox), md_file_detail.perm_check_box[6], TRUE, TRUE, 0);

    GtkWidget* g_label =  gtk_label_new("group");
    gtk_label_set_xalign(GTK_LABEL(g_label), 0.0);

    gtk_box_pack_start(GTK_BOX(g_vbox), g_label, TRUE, TRUE, 0);
    md_file_detail.perm_check_box[5] = gtk_check_button_new_with_label("read");
    md_file_detail.perm_check_box[4] = gtk_check_button_new_with_label("write");
    md_file_detail.perm_check_box[3] = gtk_check_button_new_with_label("execute");
    gtk_box_pack_start(GTK_BOX(g_vbox), md_file_detail.perm_check_box[5], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(g_vbox), md_file_detail.perm_check_box[4], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(g_vbox), md_file_detail.perm_check_box[3], TRUE, TRUE, 0);

    GtkWidget* o_label =  gtk_label_new("others");
    gtk_label_set_xalign(GTK_LABEL(o_label), 0.0);

    gtk_box_pack_start(GTK_BOX(o_vbox), o_label, TRUE, TRUE, 10);
    md_file_detail.perm_check_box[2] = gtk_check_button_new_with_label("read");
    md_file_detail.perm_check_box[1] = gtk_check_button_new_with_label("write");
    md_file_detail.perm_check_box[0] = gtk_check_button_new_with_label("execute");
    gtk_box_pack_start(GTK_BOX(o_vbox), md_file_detail.perm_check_box[2], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(o_vbox), md_file_detail.perm_check_box[1], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(o_vbox), md_file_detail.perm_check_box[0], TRUE, TRUE, 0);

    // ctrl_box
    md_file_detail.btn_close = gtk_button_new_with_label("Cancel");
    md_file_detail.btn_submit = gtk_button_new_with_label("Save");
    g_signal_connect(md_file_detail.btn_close, "clicked", G_CALLBACK(on_popup_close), md_file_detail.window);
    g_signal_connect(md_file_detail.btn_submit, "clicked", G_CALLBACK(g_callback_submit_change_perm), NULL);

    gtk_box_pack_start(GTK_BOX(ctrl_hbox), md_file_detail.btn_submit, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(ctrl_hbox), md_file_detail.btn_close, TRUE, TRUE, 5);
}

void init_text_editor_modal(GtkWidget* parent){
    int MAX_WIDTH = 600;
    int MAX_HEIGHT = 400;
    // add_btn
    md_text_editor.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(md_text_editor.window), "Text Editor");
    gtk_window_set_position(GTK_WINDOW(md_text_editor.window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(md_text_editor.window), MAX_WIDTH, MAX_HEIGHT);
    
    gtk_window_set_transient_for(GTK_WINDOW(md_text_editor.window), GTK_WINDOW(parent));
    gtk_window_set_modal(GTK_WINDOW(md_text_editor.window), TRUE);

    // layout
    GtkWidget* vbox, *func_vbox, *ctrl_hbox;
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(md_text_editor.window), vbox);

    func_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(func_vbox, MAX_WIDTH, 350);

    ctrl_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(ctrl_hbox, MAX_WIDTH, 50);

    gtk_box_pack_start(GTK_BOX(vbox), func_vbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), ctrl_hbox, TRUE, TRUE, 0);

    // func_vbox
    GtkWidget* scrolled_window, *text_vbox;
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolled_window, MAX_WIDTH, 300);
    gtk_container_add(GTK_CONTAINER(func_vbox), scrolled_window);
    
    text_vbox =  gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    // gtk_widget_set_size_request(text_vbox, MAX_WIDTH, 350);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_vbox);
    
    md_text_editor.textview = gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(text_vbox), md_text_editor.textview, TRUE, TRUE, 0);

    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(md_text_editor.textview), GTK_WRAP_WORD);


    // ctrl_box
    GtkWidget* btn_close, *btn_submit;
    btn_close = gtk_button_new_with_label("Cancel");
    btn_submit = gtk_button_new_with_label("Save");
    g_signal_connect(btn_close, "clicked", G_CALLBACK(on_popup_close), md_text_editor.window);
    g_signal_connect(btn_submit, "clicked", G_CALLBACK(g_callback_submit_mkfile), NULL);

    gtk_box_pack_start(GTK_BOX(ctrl_hbox), btn_submit, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(ctrl_hbox), btn_close, TRUE, TRUE, 5);

}

void init_modal(GtkWidget* parent){
    init_mkdir_modal(parent);
    init_modal_file_detail(parent);
    init_file_menu_context(parent);
    init_dir_menu_context(parent);
    init_bg_menu_context(parent);
    init_text_editor_modal(parent);
}
