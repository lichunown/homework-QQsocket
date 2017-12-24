#include "my_cli_head.h"


void get_sock()
{
    char * now_time = time_now();
    GtkTextIter iter;
    void* data;
    int datalen;
    struct server_to_client_send_to_user_head* headdata;
    int cou = 0;
  while(1)
 {

   printf("%d\n",cou); cou++;
    struct HEAD_RETURN* head_return = client_recv_HEAD_RETURN(sockfd);
	int returnmode = 0;
	switch(head_return->mode){
		case 11://login
			if(head_return->succ==0){
				message_out("[msg]:   login successfull.");
                struct server_login_return* data = client_recv_login_return(sockfd);
                gdk_threads_enter();
                log_token = malloc(TOKENSIZE);
                gdk_threads_leave();
                strcpy(log_token, data->token);
                //main_interface();
                //gtk_widget_destroy(GTK_WIDGET(log_win));
				returnmode = 1;
			}else{
				dailog_log_win("登录错误");
				returnmode = -1;
			}
			break;
		case 12://signup
			if(head_return->succ==0){
                 dailog_log_win("注册成功");
				//message_out("[msg]:   signup successfull.");
			}else{
                 dailog_log_win("注册错误");
				//message_out("[error]: signup error.");
				returnmode = -1;
			}		
			break;
		case 13://another user login
			//message_out("[error]: another user login.");
            //returnmode = -1;
            dailog_log_win("你的账户在其他地点登录");
            sleep(3);
            exit(0);
			break;
		case 21://logout（登出）
			if(head_return->succ==0){
				message_out("[msg]:   logout successfull.");
				returnmode = -1;
			}
			break;
		case 22://showlist
			/*for(int i=0;i < (head_return->datalen)/sizeof(list_per_user);i++){
				client_recv_and_out_perlist(sockfd);
            }*/
            for(int i=0;i < (head_return->datalen)/sizeof(list_per_user);i++){
            struct list_per_user* data = client_recv_list_return(sockfd);
            gtk_tree_store_append(treestore, &child, &toplevel);
            gtk_tree_store_set(treestore, &child,COLUMN,data->username, -1);
            free(data);
        }       


			break;
		case 20://send succ
			if(head_return->succ==0){
				message_out("[msg]:   send successfull.");
			}else{
				message_out("[warning]: send error.");
			}
			break;
		case 99://recv data
            //client_recv_and_out_data(sockfd);
            headdata = client_recv_data_return(sockfd);
            datalen = headdata->len;
            data = malloc(datalen);
            Recv(sockfd,data,datalen,0);

            memset(&getbuf,0,sizeof(getbuf));
            sprintf(getbuf,"%s %s:%s\n",now_time,headdata->username,data);

            gdk_threads_enter();
            gtk_text_buffer_get_end_iter(buffer_c,&iter);
            gtk_text_buffer_insert(buffer_c,&iter,getbuf,-1);
            gdk_threads_leave();

            free(headdata);
            free(data);
			break;		
		case 50://token error
			message_out("[error]: token error.\n\ttry login again.");
			returnmode = -1;
			break;					
		default:
			break;
        }
     free(head_return);

   }   
	
	//return returnmode;




}