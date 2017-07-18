/*******************************************************************************
 * Author :          Sequin_YF
 * Email :           catherine199787@outlook.com
 * Last modified :   2016-08-04 16:20
 * Filename :        study.c
 * Description :
 * *****************************************************************************/

#include<gtk/gtk.h>

void hello(GtkWidget *widget, gpointer *data){
    g_print("button clicked and data = %s \n",(char*)data);
}

void destroy(GtkWidget *widget, gpointer * data){
    gtk_main_quit();
}

int main(int argc, char *argv[]){
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *box;
    GtkWidget *group;
    GtkWidget *check ,*radio;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);      //创建顶级窗口

    //关闭窗口时，执行回调函数
    g_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(destroy), NULL);

    gtk_container_border_width(GTK_CONTAINER(window),100); //边框宽度

    //生成垂直容器
    box = gtk_vbox(FALSE, 0);
    gtk_container_add(GTK_CONTATINER(window), box);

    //生成两个垂直按钮
    check = gtk_check_button_new_with_label("silly A");  //创建带文本的标签

    //单击按钮时，执行回调函数
    g_signal_connect (GTK_OBJECT(check), "clicked", GTK_SIGNAL_FUNC(clicked_button), "check button1");

    //加入box中
    gtk_box_pack_start(GTK_BOX(box), check, TRUE, TRUE, 0);
    //gtk_container_add(GTK_CONTAINER(window), button);  //将按钮添加到窗口中

    gtk_widget_show(check);  //显示控件

    check = gtk_check_button_new_label("silly B");
    g_signal_connect(GTK_OBJECT(check), "clicked", GTK_SINGAL_FUNC(click_button), "check button2");
    gtk_box_pack_start(GTK_BOX(box), check, TRUE, TRUEM 0);
    gtk_widget_show(check);


    //三个radio
    radio = gtk_radio_button_new_label(NULL, "1");
    g_signal_connect(GTK_OBJECT(radio), "clicked", GTK_SIGNAL_FUNC(click_button), "1");

    gtk_box_pack_start(GTK_BOX(box), radio, TRUN, TRUN, 0);
    gtk_widget_show(radio);

    //生成第一个radio按钮时group参数指向NULL， 此后都要重新获取group
    group = gtk_radio_button_group(GTK_RADIO_BUTTON(radio));
    radio = gtk_radio_button_new_label(group, "2");
    g_signal_connect(GTK_OBJET(box),"clicked", GTK_SIGNAL_FUNC(click_button), "2")
    gtk_box_pack_start(GTK_BOX(box), TRUN, TRUN, 0);
    gtk_widget_show(radio);

    group = gtk_radio_buttun_group(GTK_RADIO_BUTTON(radio));
    radio = gtk_radio_button_new_label(group, "3");
    g_signal_connect(GTK_OBJECT(radio), "clicked", GTK_SIGNAL_FUNC(click_button), "3");

    //将第三个按钮设置为选中状态
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio), TRUN);
    gtk_box_pack_start(GET_)
    gtk_widget_show(window);

    gtk_main();  //使GTK进入消息处理循环

    return 0;
}

