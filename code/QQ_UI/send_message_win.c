#include "my_cli_head.h" 
GtkWidget *window_c;       /*单聊窗口*/
GtkWidget *window1;      /*聊天记录窗口*/
GtkWidget *window1_g;      /*聊天记录窗口*/
GtkWidget *window_g;     /*群聊窗口*/

GtkWidget *dailog_win;    /*错误提示窗口*/


GtkWidget *message_entry;


GtkWidget *message_entry_g;
GtkTextBuffer *buffer_g;

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
/*void get_message(void)
{
   while(1){
    ///if(lock==1) sleep(1);
    char * now_time = time_now();
    GtkTextIter iter;
    //gdk_threads_enter();
    struct HEAD_RETURN* head_return = client_recv_HEAD_RETURN(sockfd);
    //gdk_threads_leave();
    int returnmode = 0;
    int datalen;
    int i;
    void* data;
    struct list_per_user* list_per_userdata;
    struct server_to_client_send_to_user_head* headdata;
	switch(head_return->mode){
		case 13://another user login
			message_out("[error]: another user login.");
			returnmode = -1;
			break;
		case 20://send succ
			if(head_return->succ==0){
				message_out("[msg]:   send successfull.");
			}else{
				message_out("[warning]: send error.");
			}
            break;
		case 22://showlist
			for(i=0;i < (head_return->datalen)/sizeof(list_per_user);i++){
                list_per_userdata = client_recv_list_return(sockfd);
                gtk_tree_store_append(treestore, &child, &toplevel);
                gtk_tree_store_set(treestore, &child,COLUMN,list_per_userdata->username, -1);
                free(list_per_userdata);
			}
			break;            
		case 99://recv data
            headdata = client_recv_data_return(sockfd);
            datalen = headdata->len;
            data = malloc(datalen);
            Recv(sockfd,data,datalen,0);

            memset(&getbuf,0,sizeof(getbuf));
            sprintf(getbuf,"%s %s:%s\n",now_time,headdata->username,data);

           
            gtk_text_buffer_get_end_iter(buffer_c,&iter);
            gtk_text_buffer_insert(buffer_c,&iter,sendbuf,-1);


            free(headdata);
            free(data);
			break;							
		default:
			break;
        }
    }

}*/



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


    struct HEAD_MAIN head_main;
    head_main.mode = 1;
    Send(sockfd,&head_main,sizeof(head_main),0);
    gdk_threads_enter();
    struct HEAD_DATA* data_head = data_sendto(log_token);
    gdk_threads_leave();
    Send(sockfd,data_head,sizeof(struct HEAD_DATA),0);
    free(data_head);
    printf("sending to `%s`\n",name);
    struct client_to_server_send_to_user_head* send_head = data_sendto_head(name,strlen(message)+1);
    Send(sockfd,send_head,sizeof(struct client_to_server_send_to_user_head),0);
    Send(sockfd,message,strlen(message)+1,0);
    free(send_head);


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
    /*------ 多线程操作接受消息------------------------*/
    
  //  fork();
}
