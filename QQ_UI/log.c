
#include "my_cli_head.h" 
#include "chat_block.c"
GtkWidget *dailog_win;
GtkWidget *main_win;
GtkWidget *rig_win;
static GtkWidget *user;
static GtkWidget *passwd;
static GtkWidget *passwd1;
static GtkWidget *passwd2;
static GtkWidget *answer;




/*******************************************************************
函    数:       display_rig_win
功    能:       关闭注册错误提示框
传入参数:        无
传出参数:        无
返    回:        无
call            NULL
called by        dailog_rig_win
********************************************************************/
void display_rig_win()
{
    gtk_widget_destroy(GTK_WIDGET(dailog_win));
}


/*******************************************************************
函    数:       display_rig_win
功    能:       提示注册错误信息
传入参数:        buf_d
传出参数:        无
返    回:       无
call            display_rig_win()
called  by      resrve_user_name() 
********************************************************************/
/*注册错误的对话提示窗口*/
void dailog_rig_win(char * buf_d)
{
    GdkPixbuf *pixbuf = NULL;
    GdkBitmap *bitmap = NULL;
    GdkPixmap *pixmap = NULL;
    GtkWidget *vbox,*hbox,*button1,*label;

    /*创建提示窗口*/
    dailog_win=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_decorated(GTK_WINDOW(dailog_win), FALSE);// 设置无边框
    gtk_window_set_title(GTK_WINDOW(dailog_win),"提示");
    gtk_widget_set_usize(dailog_win,250,150);
	gtk_widget_set_uposition(dailog_win,500,300);
    gtk_container_set_border_width(GTK_CONTAINER(dailog_win),10);

    /*设置提示窗口背景色*/
    gtk_widget_set_app_paintable(dailog_win, TRUE);
    gtk_widget_realize(dailog_win);
    pixbuf = gdk_pixbuf_new_from_file("xiaohai.jpg", NULL);// gdk 函数读取 png 文件
    gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, &bitmap, 128);// alpha 小于 128 认为透明
    gtk_widget_shape_combine_mask(dailog_win, bitmap, 0, 0);   // 设置透明蒙板
    gdk_window_set_back_pixmap(dailog_win->window, pixmap, FALSE); // 设置窗口背景

    vbox=gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(dailog_win),vbox);
    hbox=gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,30);

    /*设置提示信息*/
    label=gtk_label_new(buf_d);
    gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,FALSE,5);
    hbox=gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
    button1=gtk_button_new_with_label("确定");
    gtk_box_pack_start(GTK_BOX(hbox),button1,TRUE,TRUE,5);

    g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(display_rig_win),NULL);

    gtk_widget_show_all(dailog_win);
}

/*******************************************************************
函    数:       resrve_user_name
功    能:       把用户注册的信息发送到服务器并保存
传入参数:       rig_win
传出参数:       无
返    回:       无
call           dailog_rig_win 如注册出现错误，可调用此函数弹出提示
called by       rig_user
注释            注册所需的id，密码，确认密码是全局变量，因此次函数没有参数
********************************************************************/
void resrve_user_name()
{
    
}







/*******************************************************************
函    数:         quit_rig_interface
功    能:         取消注册
传入参数:    无
传出参数:    无
返    回:        无
call          null
called by    rig_user
********************************************************************/
/*取消注册*/
void quit_rig_interface()
{
    gtk_widget_destroy(GTK_WIDGET(rig_win));
    log_user_interface();
}



/***********************************************************************
  函数:            rig_user
  功能:           注册用户界面
  Calls:          quit_rig_interface；
                  resrve_user_name;

  Called By:      log_user_interface

***********************************************************************/
/*注册用户界面*/
void rig_user()
{

    GdkPixbuf *pixbuf = NULL;
    GdkBitmap *bitmap = NULL;
    GdkPixmap *pixmap = NULL;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *label;
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *sep;
    GtkWidget *combo;
    GList *items = NULL;
    //GtkWidget *item;

    /*关闭登录窗口*/
    gtk_widget_destroy(GTK_WIDGET(log_win));

    /*创建注册窗口*/
    rig_win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize(rig_win,250,350);
	gtk_widget_set_uposition(rig_win,50,30);
    gtk_window_set_title(GTK_WINDOW(rig_win),"注册");
    gtk_window_set_position(GTK_WINDOW(rig_win),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(rig_win),20);//设置边界宽度
    gtk_window_set_decorated(GTK_WINDOW(rig_win), FALSE);       // 设置无边框

    /*设置窗口背景色*/
    gtk_widget_set_app_paintable(rig_win, TRUE);
    gtk_widget_realize(rig_win);
    pixbuf = gdk_pixbuf_new_from_file("xiaohai.jpg", NULL);       // gdk 函数读取 png 文件
    gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, &bitmap, 128);// alpha 小于 128 认为透明
    gtk_widget_shape_combine_mask(rig_win, bitmap, 0, 0);          // 设置透明蒙板
    gdk_window_set_back_pixmap(rig_win->window, pixmap, FALSE);          // 设置窗口背景

    vbox = gtk_vbox_new(FALSE,0);    //创建一个纵向盒子
    gtk_container_add(GTK_CONTAINER(rig_win),vbox);  //纵向盒子组见到主窗口
    hbox = gtk_hbox_new(FALSE,0);    //创建一个横向盒子
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
    label = gtk_label_new("用 户 名:");
    gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,FALSE,5);
    user = gtk_entry_new();//设置帐号输入区
    gtk_box_pack_start(GTK_BOX(hbox),user,FALSE,FALSE,0);

	hbox = gtk_hbox_new(FALSE,0);   //创建一个横向盒子
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
    label = gtk_label_new("密    码: ");
    gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,FALSE,5);
    passwd1 = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(passwd1),FALSE);//设置输入密码时不会显示输入的字符
    gtk_box_pack_start(GTK_BOX(hbox),passwd1,FALSE,FALSE,0);

	hbox = gtk_hbox_new(FALSE,0);   //创建一个横向盒子
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
    label = gtk_label_new("确认密码: ");

    gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,FALSE,0);
    passwd2 = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(passwd2),FALSE);//设置输入密码时不会显示输入的字符
    gtk_box_pack_start(GTK_BOX(hbox),passwd2,FALSE,FALSE,0);


    sep = gtk_hseparator_new();   //设置分隔线
    gtk_box_pack_start(GTK_BOX(vbox),sep,TRUE,FALSE,5);

	hbox = gtk_hbox_new(FALSE,0);   //创建一个横向盒子
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,10);
	button1 = gtk_button_new_with_label("立即注册");
    gtk_box_pack_start(GTK_BOX(hbox),button1,FALSE,FALSE,20);
	button2 = gtk_button_new_with_label("退出");
    gtk_box_pack_start(GTK_BOX(hbox),button2,FALSE,FALSE,20);

	g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(resrve_user_name),NULL);
	g_signal_connect(G_OBJECT(button2),"clicked",G_CALLBACK(quit_rig_interface),NULL);

    gtk_widget_show_all(rig_win);

}


/*******************************************************************
函    数:       display_log_win
功    能:       关闭登录错误的提示窗口
传入参数:        无
传出参数:        无
返    回:        无
call            NULL
called by       dailog_log_win
********************************************************************/
void display_log_win()
{
    gtk_widget_destroy(GTK_WIDGET(dailog_win));
}

/*登录错误的对话提示窗口*/
void dailog_log_win(char * buf_d)
{
    GdkPixbuf *pixbuf = NULL;
    GdkBitmap *bitmap = NULL;
    GdkPixmap *pixmap = NULL;
    GtkWidget *vbox,*hbox,*button1,*label;

    /*创建提示窗口*/
    dailog_win=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_decorated(GTK_WINDOW(dailog_win), FALSE);// 设置无边框
    gtk_window_set_title(GTK_WINDOW(dailog_win),"提示");
    gtk_widget_set_usize(dailog_win,250,150);
	gtk_widget_set_uposition(dailog_win,500,300);
    gtk_container_set_border_width(GTK_CONTAINER(dailog_win),10);

    /*设置提示窗口背景色*/
    gtk_widget_set_app_paintable(dailog_win, TRUE);
    gtk_widget_realize(dailog_win);
    pixbuf = gdk_pixbuf_new_from_file("xiaohai.jpg", NULL);// gdk 函数读取 png 文件
    gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, &bitmap, 128);// alpha 小于 128 认为透明
    gtk_widget_shape_combine_mask(dailog_win, bitmap, 0, 0);   // 设置透明蒙板
    gdk_window_set_back_pixmap(dailog_win->window, pixmap, FALSE); // 设置窗口背景

    vbox=gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(dailog_win),vbox);
    hbox=gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,30);

    /*设置提示信息*/
    label=gtk_label_new(buf_d);
    gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,FALSE,5);
    hbox=gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
    button1=gtk_button_new_with_label("确定");
    gtk_box_pack_start(GTK_BOX(hbox),button1,TRUE,TRUE,5);

    g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(display_log_win),NULL);

    gtk_widget_show_all(dailog_win);
}



/***********************************************************************
  函数:           log_user
  功能:            登录用户验证用户名和密码并进入主界面
  Calls:          main_interface();登录成功，调入主界面函数
                  dailog_log_win() 错误信息提示函数
  Called By:      log_user_interface；被登录主界面函数回掉
  
***********************************************************************/

/*登录用户验证用户名和密码*/
void log_user()
{
   
     
    /*-------------test-----测试程序：直接进入主界面-------------------*/
        main_interface();
        gtk_widget_destroy(GTK_WIDGET(log_win));
    /*-------------------------------------------*/
    
}



/*******************************************************************
函    数:       quit();
功    能:        退出登录界面
传入参数:    无
传出参数:    无
返    回:        无
********************************************************************/
/*退出登录界面*/
void quit()
{
    gtk_main_quit();
}



/*******************************************************************
函    数:    log_user_interface
功    能:    绘制登录窗口
传入参数:    无
传出参数:    无
返    回:    0:程序正常退出
             1:程序错误退出
called by   main()
call        rig_user()
            log_user()
            quit()
********************************************************************/
void log_user_interface()
{
    GdkPixbuf *pixbuf = NULL;
    GdkBitmap *bitmap = NULL;
    GdkPixmap *pixmap = NULL;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *label;
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *button3;
    GtkWidget *sep;

    /*创建主窗口*/
    log_win=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_decorated(GTK_WINDOW(log_win), FALSE);// 设置无边框
    gtk_widget_set_usize(log_win,250,150);
	gtk_widget_set_uposition(log_win,500,300);
    gtk_window_set_title(GTK_WINDOW(log_win),"登录");
    gtk_window_set_position(GTK_WINDOW(log_win),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(log_win),20);//设置边界宽度

    /*设置背景图片*/
    gtk_widget_set_app_paintable(log_win, TRUE);
    gtk_widget_realize(log_win);
    pixbuf = gdk_pixbuf_new_from_file("xiaohai.jpg", NULL);// gdk 函数读取 png 文件
    gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, &bitmap, 128);// alpha 小于 128 认为透明
    gtk_widget_shape_combine_mask(log_win, bitmap, 0, 0);   // 设置透明蒙板
    gdk_window_set_back_pixmap(log_win->window, pixmap, FALSE); // 设置窗口背景


	vbox=gtk_vbox_new(FALSE,0);    //创建一个纵向盒子
    gtk_container_add(GTK_CONTAINER(log_win),vbox);  //纵向盒子组见到主窗口

    hbox=gtk_hbox_new(FALSE,0);    //创建一个横向盒子
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
    label=gtk_label_new("帐号:");
    user=gtk_entry_new();          //设置帐号输入区
    gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(hbox),user,FALSE,FALSE,0);

	hbox=gtk_hbox_new(FALSE,0);   //创建一个横向盒子
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
    label=gtk_label_new("密码: ");
    passwd=gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(passwd),FALSE);//设置输入密码时不会显示输入的字符
    gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(hbox),passwd,FALSE,FALSE,0);

	sep=gtk_hseparator_new();   //设置分隔线
    gtk_box_pack_start(GTK_BOX(vbox),sep,FALSE,FALSE,5);

	hbox=gtk_hbox_new(FALSE,0);   //创建一个横向盒子
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,10);
	button1=gtk_button_new_with_label("注册");
    gtk_box_pack_start(GTK_BOX(hbox),button1,FALSE,FALSE,15);

    gtk_widget_realize(hbox);
    gdk_window_set_cursor(hbox->window,gdk_cursor_new(GDK_HAND1));

	button2=gtk_button_new_with_label("登录");
    gtk_box_pack_start(GTK_BOX(hbox),button2,FALSE,FALSE,15);

	button3=gtk_button_new_with_label("退出");
    gtk_box_pack_start(GTK_BOX(hbox),button3,FALSE,FALSE,15);

    g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(rig_user),log_win);
    g_signal_connect(G_OBJECT(button2),"clicked",G_CALLBACK(log_user),log_win);
    g_signal_connect(G_OBJECT(button3),"clicked",G_CALLBACK(quit),log_win);

    gtk_widget_show_all(log_win);

}
