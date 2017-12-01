#include "my_cli_head.h" 
GtkWidget *window_c;       /*单聊窗口*/
GtkWidget *window1;      /*聊天记录窗口*/
GtkWidget *window1_g;      /*聊天记录窗口*/
GtkWidget *window_g;     /*群聊窗口*/

GtkWidget *dailog_win;    /*错误提示窗口*/


GtkWidget *message_entry;
GtkTextBuffer *buffer_c;
char sendbuf[1024],getbuf[1024];
GtkWidget *message_entry_g;
GtkTextBuffer *buffer_g;
char sendbuf_g[1024],getbuf_g[1024];
int count = 0;




/*******************************************************************
函    数:      void on_delete_event(GtkWidget *widget,GdkEvent *event,gpointer data)
功    能:       退出聊天窗口
传入参数:    无
传出参数:    无
返    回:        无
********************************************************************/
/*退出聊天窗口*/
void on_delete_event(GtkWidget *widget,GdkEvent *event,gpointer data)
{
    gtk_widget_destroy(GTK_WIDGET(window_c));
}


/*******************************************************************
函    数:      char * time_now()
功    能:       记录消息发送的时间
传入参数:    无
传出参数:    无
返    回:        无
********************************************************************/
char * time_now()
{
    time_t now;
    struct tm * timenow;
   // char strtemp[255];
    time(&now);
    timenow = localtime(&now);
   // char * now_time = asctime(timenow);
    return (asctime(timenow));
}





/*******************************************************************
函    数:     void get_message(void)
功    能:       接收单聊消息函数
传入参数:    无
传出参数:    无
返    回:        无
********************************************************************/
/*接收单聊消息*/
void get_message(void)
{
  
}



/*******************************************************************
函    数:     void on_send ()
功    能:       发送单聊消息函数
传入参数:    无
传出参数:    无
返    回:        无
********************************************************************/
/*发送单聊消息*/
void on_send ()
{
    
    /*-----test---测试：只将要发送的消息传到文本框，未经过服务器-------*/
    static  sqlite3 *db=NULL;
    static char *errmsg=NULL;
    int rc;
    char buf[1024];
    char * log_username = (char *)malloc(10);
    int result = 0;
    const char *message;GtkTextIter iter;
    char * now_time = time_now();
    message=gtk_entry_get_text(GTK_ENTRY(message_entry));

    memset(&sendbuf,0,sizeof(sendbuf));
    sprintf(sendbuf,"%s 我:%s\n",now_time,message);


    gtk_entry_set_text(GTK_ENTRY(message_entry),"");

    gdk_threads_enter();
    gtk_text_buffer_get_end_iter(buffer_c,&iter);
    gtk_text_buffer_insert(buffer_c,&iter,sendbuf,-1);
    gdk_threads_leave();
   /*-------------------------------------------------------------*/


    
}

/*******************************************************************
函    数:     void determine_delete_friends()
功    能:       确定删除好友
传入参数:    无
传出参数:    无
返    回:        无
********************************************************************/
/*确定删除好友*/
void determine_delete_friends()
{
     deleted_friends();
    
    gtk_widget_destroy(GTK_WIDGET(window_d));
}

void quit_delete_friends()
{
    gtk_widget_destroy(GTK_WIDGET(window_d));
}


/*******************************************************************
函    数:     void cancel_delete_friends()
功    能:       取消删除好友
传入参数:    无
传出参数:    无
返    回:        无
********************************************************************/
void cancel_delete_friends()
{
    send_message_win(name);
    gtk_widget_destroy(GTK_WIDGET(window_d));

}




/*******************************************************************
函    数:     void delete_friends()
功    能:       创建删除好友窗口
传入参数:    无
传出参数:    无
返    回:        无
********************************************************************/
/*删除好友窗口*/
void delete_friends()
{
    gtk_widget_destroy(GTK_WIDGET(window_c));
    char buf_d[50];
    GtkWidget *vbox,*hbox,*button1,*button2,*button3;
    window_d=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window_d),"删除好友");
    gtk_widget_set_usize(window_d,250,155);
    g_signal_connect(G_OBJECT(window_d),"delete_event",G_CALLBACK(on_delete_event),NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window_d),10);
    gtk_window_set_decorated(GTK_WINDOW(window_d), FALSE);       // 设置无边框
    vbox=gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(window_d),vbox);
    hbox=gtk_hbox_new(FALSE,0);
    sprintf(buf_d,"您确定要删除好友:%s?",name);
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,30);
    label_del = gtk_label_new(buf_d);
    gtk_box_pack_start(GTK_BOX(hbox),label_del,FALSE,FALSE,5);

    hbox=gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
    button1=gtk_button_new_with_label("是(Y)");
    gtk_box_pack_start(GTK_BOX(hbox),button1,TRUE,TRUE,5);
    button2=gtk_button_new_with_label("否(N)");
    gtk_box_pack_start(GTK_BOX(hbox),button2,TRUE,TRUE,5);
    button3=gtk_button_new_with_label("退出(N)");
    gtk_box_pack_start(GTK_BOX(hbox),button3,TRUE,TRUE,5);
    g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(determine_delete_friends),NULL);
    g_signal_connect(G_OBJECT(button2),"clicked",G_CALLBACK(cancel_delete_friends),NULL);
    g_signal_connect(G_OBJECT(button3),"clicked",G_CALLBACK(quit_delete_friends),NULL);
    gtk_widget_show_all(window_d);
}




/*******************************************************************
函    数:     void send_message_win(gchar * name)
功    能:       创建发送／接收消息窗口
传入参数:    无
传出参数:    无
返    回:        无
called by    view_onRowActivated
********************************************************************/
/*发送／接收消息窗口*/
void send_message_win(gchar * name)
{
    count++;
    GtkWidget *text;

    GtkWidget *vbox,*hbox,*button,*button1,*button2,*label,*view,*button_test;

    window_c = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window_c),name);
    gtk_widget_set_usize(window_c,400,300);
    g_signal_connect(G_OBJECT(window_c),"delete_event",G_CALLBACK(on_delete_event),NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window_c),10);
    vbox=gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(window_c),vbox);
    hbox=gtk_hbox_new(FALSE,0);

    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);

    view=gtk_scrolled_window_new(NULL,NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(view),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    text=gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(vbox),view,TRUE,TRUE,5);
    gtk_container_add(GTK_CONTAINER(view),text);//滚动条和文本框

    buffer_c=gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));


   
    

    hbox=gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);

    label=gtk_label_new("输入消息：");
    gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,5);

    message_entry=gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox),message_entry,FALSE,FALSE,5);

    button=gtk_button_new_with_label("发送");
    gtk_box_pack_start(GTK_BOX(hbox),button,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
    button1=gtk_button_new_with_label("删除好友");
    gtk_box_pack_start(GTK_BOX(hbox),button1,FALSE,FALSE,5);

    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(on_send),name);
    g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(delete_friends),NULL);

    gtk_widget_show_all(window_c);
}
