
#include "my_cli_head.h" 
#include "send_message_win.c"

int j = 10;
char buf1[20] = "";
guint timer; // 定时器id
static GtkWidget *user;
GtkWidget *addfri_win;
GtkWidget *dailog_win;
GtkWidget *alart_win;
GtkTreeModel *model;
GtkWidget *button1;
GtkTextBuffer *buffer_t;
char lable[50] ="请输入您要查找的帐号";
GtkWidget *label_c;
char lable_t[100] ="你好";
GtkWidget *label_a;



/*******************************************************************
函    数:      void quit_addfri_win()
功    能:       取消添加好友
传入参数:    无
传出参数:    无
返    回:        无
********************************************************************/
/*取消添加好友*/
void quit_addfri_win()
{
    gtk_widget_destroy(GTK_WIDGET(addfri_win));
}



/*******************************************************************
函    数:      quit_window()
功    能:       退出主窗口
传入参数:    无
传出参数:    无
返    回:        无
********************************************************************/
void quit_window()
{
    ///msg.action = 7;
    ///write(sockfd,&msg,sizeof(MSG));
    gtk_main_quit();
}

void quit_alart_win()
{
     gtk_widget_destroy(GTK_WIDGET(alart_win));
}



/*******************************************************************
函    数:      void display_friends()
功    能:       把所有好友都显示到主界面
传入参数:    无
传出参数:    无
返    回:        无
********************************************************************/
/*显示好友*/
void display_friends()
{
   struct HEAD_MAIN head_main;
   head_main.mode = 1;
   Send(sockfd, &head_main, sizeof(head_main),0);
   gdk_threads_enter();
   struct Head_DATA* senddata = data_showlist(log_token);
   gdk_threads_leave();
   Send(sockfd,senddata,sizeof(struct HEAD_DATA),0);
   free(senddata);
    /*---test----测试程序：将小马，小王添加到好友列表（假设已经从服务器获取）-----------------------*/
   /* char text_char[100] = "小马";
    char text_char2[100] = "小王";
     gtk_tree_store_append(treestore, &child, &toplevel);
     gtk_tree_store_set(treestore, &child,COLUMN,text_char, -1);
     gtk_tree_store_append(treestore, &child, &toplevel);
     gtk_tree_store_set(treestore, &child,COLUMN,text_char2, -1);*/
     /*-----------------------------------*/

}

/*******************************************************************
函    数:       void set_widget_font_size(GtkWidget *widget, int size, int is_button)
功    能:       设置标签字体
传入参数:    无
传出参数:    无
返    回:        无
********************************************************************/

void set_widget_font_size(GtkWidget *widget, int size, int is_button)
{
    GtkWidget *labelChild;
    PangoFontDescription *font;
    gint fontSize = size;

    font = pango_font_description_from_string("Sans");//"Sans"字体名
    pango_font_description_set_size(font, fontSize*PANGO_SCALE);//设置字体大小

    if(is_button)
    {
        labelChild = gtk_bin_get_child(GTK_BIN(widget));//取出GtkButton里的label
    }
    else
    {
        labelChild = widget;
    }
   // 设置label的字体，这样这个GtkButton上面显示的字体就变了
    gtk_widget_modify_font(GTK_WIDGET(labelChild), font);
    pango_font_description_free(font);
}

/*******************************************************************
函    数:       void add_friends()
功    能:        把添加好友的信息发送到服务器
传入参数:    无
传出参数:    无
返    回:        无
called by      find_friends()

call           add_success()  添加成功
               cant_add()    非好友
               no_rig_cant_add()  好友未注册
注释            好友的id在函数find_friends中存在全局变量user中，所以无参数传入
********************************************************************/
void add_friends()
{
   
    
    
   
}
/*******************************************************************
函    数:       alart_message()
功    能:       添加好友问题提示框
传入参数:       buf 存有提示信息
传出参数:       无
返    回:        无
called by      add_success()
               cant_add()
               no_rig_cant_add()
call           无
********************************************************************/
char alart_message(char buf[])
{
     GtkWidget *hbox;
    GtkWidget *vbox;
    GtkWidget *vbox2;
    GtkWidget *label;
    GtkWidget *ok_button;
    GtkWidget *sep;
    alart_win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(alart_win),"通知");
    gtk_widget_set_usize(alart_win,250,150);
    gtk_widget_set_uposition(alart_win,50,100);
    gtk_window_set_position(GTK_WINDOW(alart_win),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(alart_win),0);//设置边界宽度
    gtk_window_set_decorated(GTK_WINDOW(alart_win), TRUE);       // 设置无边框
    gtk_widget_set_app_paintable(alart_win, TRUE);
    gtk_widget_realize(alart_win);

    vbox = gtk_vbox_new(FALSE,0);    //创建一个纵向盒子
    gtk_container_add(GTK_CONTAINER(alart_win),vbox);  //纵向盒子组见到主窗口

    hbox = gtk_hbox_new(FALSE,0);    //创建一个横向盒子
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,15);
    label = gtk_label_new(buf);
    gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,FALSE,0);
    sep = gtk_hseparator_new();   //设置分隔线
    gtk_box_pack_start(GTK_BOX(vbox),sep,TRUE,FALSE,30);
    vbox2 = gtk_hbox_new(FALSE,0);   //创建一个纵向盒子
    gtk_box_pack_start(GTK_BOX(vbox),vbox2,FALSE,FALSE,5);
    ok_button = gtk_button_new_with_label("确认");
    gtk_box_pack_start(GTK_BOX(vbox2),ok_button,FALSE,FALSE,100);//将确认按钮放入盒子
    g_signal_connect(G_OBJECT(ok_button),"clicked",G_CALLBACK(quit_alart_win),NULL);
    gtk_widget_show_all(alart_win);

}
/*******************************************************************
函    数:       add_success()
功    能:       提示好友添加成功，并将新的好友加入好友列表
传入参数:       无
传出参数:       无
返    回:        无
called by      add_friends()
call           alart_message(buf);
注释            好友的id在函数find_friends中存在全局变量user中，所以无参数传入
********************************************************************/
void add_success()
{
    
    const gchar * add_user_id;
    add_user_id = gtk_entry_get_text(GTK_ENTRY(user));//user
    gtk_tree_store_append(treestore, &child, &toplevel);
    gtk_tree_store_set(treestore, &child, COLUMN,add_user_id, -1);
    char buf[100] = "添加成功";
    alart_message(buf);
    
}

/*已是您的好友不能再添加*/
void cant_add()
{
    char buf[100] = "已是您的好友不能再添加";
    alart_message(buf);
}

/*此用户没有注册*/
void no_rig_cant_add()
{
     char buf[100] = "此用户没有注册";
    alart_message(buf);
}


void delete_success()
{
    char buf_del[20];
    strcpy(buf_del,"已删除点X关闭窗口");
    gtk_label_set_text(GTK_LABEL(label_del), buf_del);
    //sleep(3);
    gtk_widget_destroy(GTK_WIDGET(window_d));
}


/*******************************************************************
函    数:       void find_friends()
功    能:        创建添加好友窗口
传入参数:    无
传出参数:    无
返    回:        无
called by   main_interface
call        add_friends
            quit_addfri_win
********************************************************************/
/*添加好友窗口*/
void find_friends()
{
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *label;
    GtkWidget *button2;
    GtkWidget *sep;
   // GtkWidget *text;
   // GtkWidget *view;
    addfri_win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_usize(addfri_win,250,150);
    gtk_widget_set_uposition(addfri_win,50,30);
    gtk_window_set_title(GTK_WINDOW(addfri_win),"添加好友");
    gtk_window_set_position(GTK_WINDOW(addfri_win),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(addfri_win),0);//设置边界宽度
    gtk_window_set_decorated(GTK_WINDOW(addfri_win), TRUE);       // 设置无边框
    gtk_widget_set_app_paintable(addfri_win, TRUE);
    gtk_widget_realize(addfri_win);

    vbox = gtk_vbox_new(FALSE,0);    //创建一个纵向盒子
    gtk_container_add(GTK_CONTAINER(addfri_win),vbox);  //纵向盒子组见到主窗口

    hbox = gtk_hbox_new(FALSE,0);    //创建一个横向盒子
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,15);
    label = gtk_label_new("查找帐号:");
    gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,FALSE,0);
    user = gtk_entry_new();//设置帐号输入区
    gtk_box_pack_start(GTK_BOX(hbox),user,FALSE,FALSE,0);

    /*设置显示标签*/
    hbox=gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
    label_c = gtk_label_new(lable);
    set_widget_font_size(label_c, 15, FALSE); // 设置label的字体大小
    gtk_container_add(GTK_CONTAINER(hbox), label_c);


    sep = gtk_hseparator_new();   //设置分隔线
    gtk_box_pack_start(GTK_BOX(vbox),sep,TRUE,FALSE,30);

    hbox = gtk_hbox_new(FALSE,0);   //创建一个横向盒子
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
    button1 = gtk_button_new_with_label("添加");
    gtk_box_pack_start(GTK_BOX(hbox),button1,FALSE,FALSE,35);
    button2 = gtk_button_new_with_label("取消");
    gtk_box_pack_start(GTK_BOX(hbox),button2,FALSE,FALSE,35);

    g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(add_friends),NULL);
    g_signal_connect(G_OBJECT(button2),"clicked",G_CALLBACK(quit_addfri_win),NULL);
    gtk_widget_show_all(addfri_win);

}



/*******************************************************************
函    数:       void view_onRowActivated (GtkTreeView *treeview,GtkTreePath *path)
功    能:        双击联系人弹出聊天窗口
传入参数:        GtkTreeView *treeview,GtkTreePath *path
传出参数:        无
返    回:        无
call         send_message_win(name);
called  by    main_interface
********************************************************************/

void view_onRowActivated (GtkTreeView *treeview,GtkTreePath *path )
{

    GtkTreeModel *model;

    g_print ("A row has been double-clicked!\n");

    model = gtk_tree_view_get_model(treeview);

    if (gtk_tree_model_get_iter(model, &child, path))
    {
        gtk_tree_model_get(model, &child, COLUMN, &name, -1);

        send_message_win(name);

    }
}




/*******************************************************************
函    数:       static GtkTreeModel *create_and_fill_model(void)
功    能:        为主界面填充视图
传入参数:    无
传出参数:    无
返    回:        无
called by    create_view_and_model
********************************************************************/
static GtkTreeModel *create_and_fill_model(void)
{
    treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING);
    gtk_tree_store_append(treestore, &toplevel, NULL);
    gtk_tree_store_set(treestore, &toplevel, COLUMN, "我的好友", -1);
    display_friends();
    return GTK_TREE_MODEL(treestore);
}



/*******************************************************************
函    数:       static GtkWidget *create_view_and_model(void)
功    能:        为主界面创建一个模式视图
传入参数:    无
传出参数:    无
返    回:        无
called by   main_interface
call        create_and_fill_model
********************************************************************/
static GtkWidget *create_view_and_model(void)
{
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;
    GtkWidget *view;

    view = gtk_tree_view_new();

    col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(col, "好友列表");
    gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer, "text", COLUMN);
    model = create_and_fill_model();
    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
    g_object_unref(model);

    return view;
}
/*******************************************************************
函    数:       deleted_friends
功    能:       移除列表中的好友
传入参数:    无
传出参数:    无
返    回:        无
called by   determine_delete_friends
call        null
注释          需要移除的id是全局变量name，该函数只负责界面的移除，服务器移除代码在determine_delete_friends中
********************************************************************/
void deleted_friends()
{
    char *str = name;
    gtk_tree_model_get(model,&child,COLUMN,&str,-1);
    if(strcmp(name,str) == 0)
    {
        gtk_tree_store_remove(treestore,&child);

    }
}


/*******************************************************************
函    数:       main_interface()
功    能:        程序主界面
传入参数:    无
传出参数:    无
返    回:        无
called by    log_user  
********************************************************************/
void main_interface()
{

    GtkTreeSelection *selection;
    GtkWidget *vbox;
   // GtkWidget *statusbar;
    GtkWidget *scrlled_window;
   // GtkWidget *scrlled_win;
    GtkWidget *vPaned;
   // GtkWidget *sep;
    GtkWidget *add;
    GtkWidget *refresh;
    GtkWidget *quit;
    GtkWidget *hbox;
    name = (gchar*)malloc(50);
    strcpy(name,"0");

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_decorated(GTK_WINDOW(window),TRUE);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "微Q");
    gtk_widget_set_size_request(window, 230, 600);

    vPaned = gtk_vpaned_new();               //创建一个分隔板
    /*创建一个滚动窗口*/
    scrlled_window = gtk_scrolled_window_new(NULL,NULL);
    gtk_container_set_border_width(GTK_CONTAINER(scrlled_window),0);
    /*滚动条的出现方式：
    *      *    GTK_POLICY_AUTOMATIC：自动决定是否需要出现滚动条
    *           *    GTK_POLICY_ALWAYS：一直显示滚动条
    *                * */
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrlled_window),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    gtk_paned_pack1(GTK_PANED(vPaned),scrlled_window, TRUE, FALSE);

    //scrlled_win = gtk_scrolled_window_new(NULL,NULL);
   // gtk_container_set_border_width(GTK_CONTAINER(scrlled_win),0);
    /*滚动条的出现方式：
    *      *    GTK_POLICY_AUTOMATIC：自动决定是否需要出现滚动条
    *           *    GTK_POLICY_ALWAYS：一直显示滚动条
    *                * */
   // gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrlled_win),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
   // gtk_paned_pack2(GTK_PANED(vPaned),scrlled_win, FALSE, FALSE);


   gtk_container_add(GTK_CONTAINER(window),vPaned);

    gtk_widget_show(scrlled_window);

    vbox = gtk_vbox_new(FALSE, 2);
    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrlled_window), vbox);
    /*创建一个模式视图*/
    view_y = create_view_and_model();
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view_y));
    gtk_box_pack_start(GTK_BOX(vbox), view_y, TRUE,TRUE, 1);

   // statusbar = gtk_statusbar_new();
   // gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, TRUE, 1);

   // gtk_widget_show(scrlled_win);

    vbox = gtk_vbox_new(FALSE, 2);
   // gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrlled_win), vbox);
    gtk_paned_pack2(GTK_PANED(vPaned),vbox, FALSE, FALSE);

/*    //设置显示标签
    hbox=gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,0);
    label_a = gtk_label_new(lable_t);
    gtk_label_set_justify(GTK_LABEL(label_a),GTK_JUSTIFY_CENTER);//设置标号对齐方式为居中对齐
    set_widget_font_size(label_a, 8, FALSE); // 设置label的字体大小
    gtk_container_add(GTK_CONTAINER(hbox), label_a);

    sep = gtk_hseparator_new();   //设置分隔线
    gtk_box_pack_start(GTK_BOX(vbox),sep,TRUE,FALSE,0);
*/
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,0);
    add = gtk_button_new_with_label("添加好友");
    refresh = gtk_button_new_with_label("刷新好友");
    quit = gtk_button_new_with_label("退出");
    gtk_box_pack_start(GTK_BOX(hbox), add, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), refresh, TRUE, TRUE, 15);
    gtk_box_pack_start(GTK_BOX(hbox), quit, TRUE, TRUE, 15);

    //pthread_create(&id,NULL,(void *)get_message,NULL);
    g_signal_connect(view_y, "row-activated", (GCallback) view_onRowActivated, NULL);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(quit_window),NULL);
    g_signal_connect(G_OBJECT(refresh),"clicked",G_CALLBACK(display_friends),NULL);
    g_signal_connect(G_OBJECT(quit),"clicked",G_CALLBACK(quit_window),NULL);
    g_signal_connect(G_OBJECT(add),"clicked",G_CALLBACK(find_friends),NULL);


    gtk_widget_show_all(window);

}
