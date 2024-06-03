#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <wiringPi.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include "client_network.h"
#include "modified_linetracing.h"


typedef struct{
    int visited;
    int has_bomb;
    int parent_pos[2];
}bfs_node;

bfs_node bfs_nodes[5][5]; 

DGIST dgist;

int next_action = 0;
int curr_direction = 0;
int curr_node[2] = {-1,-1};
int before_node = 1;
int leftout, leftin, rightin, rightout;
int flag = 0;
int sock;
pthread_mutex_t lock;
int dgist_refreshed = 0;
int send_data_flag = 0;
int action[3];
//int target_node_queue[4][2];
int file;
int qr_changed = 0;
int qr;

typedef struct action_queue_node action_queue_node;

struct action_queue_node{
    int action;
    action_queue_node *next_node;
};

action_queue_node *queue_start;
action_queue_node *queue_end;


void queue_append(int action){
    action_queue_node *new_node;
    new_node = (action_queue_node *)malloc(sizeof(new_node));
    new_node->action = action; 
    new_node->next_node = NULL;
    if(queue_start == NULL){
        queue_start = new_node;
    }
    else{
        queue_end->next_node = new_node;
    }
    queue_end = new_node;
}

int queue_pop(){
    if(queue_start == NULL){
        return -2;
    }
    else{
        int temp_int = queue_start->action;
        action_queue_node *temp_ptr = queue_start; 
        queue_start = queue_start->next_node;
        free(temp_ptr);
        return temp_int;
    }
}


void send_data(int row, int col, int action_type){
    action[0] = row;
    action[1] = col;
    action[2] = action_type;
    send_data_flag = 1;
}


// int bfs(int target_x, int target_y){
//     printf("bfs called!\n");
//     for(int i = 0;i < 5; i++){
//         for(int j = 0;j < 5; j++){
//             printf("here?\n");
//             bfs_nodes[i][j].parent_pos[0] = -2;
//             bfs_nodes[i][j].parent_pos[1] = -2;
//             bfs_nodes[i][j].visited = 0;
//             bfs_nodes[i][j].has_bomb = (dgist.map[i][j].item.status == 2) ? 1 : 0 ;
//         }
//     }
//     printf("bfs init stage passed");
//     bfs_nodes[curr_node[0]][curr_node[1]].visited = 1;
//     int bfs_flag = 1;
//     while(bfs_flag){
//         bfs_flag = 0;
//         for(int i = 0; i < 5; i++){
//             for(int j = 0; i < 5; j++){
//                 if(bfs_nodes[i][j].visited == 0){
//                     if(i >  0){
//                         if(bfs_nodes[i-1][j].visited && bfs_nodes[i-1][j].has_bomb == 0){
//                             bfs_nodes[i][j].parent_pos[0] = i-1;
//                             bfs_nodes[i][j].parent_pos[1] = j;
//                             bfs_nodes[i][j].visited = 1;
//                             bfs_flag = 1;
//                         }
//                     }
//                     else if(i < 4){
//                         if(bfs_nodes[i+1][j].visited && bfs_nodes[i+1][j].has_bomb == 0){
//                             bfs_nodes[i][j].parent_pos[0] = i + 1;
//                             bfs_nodes[i][j].parent_pos[1] = j;
//                             bfs_nodes[i][j].visited = 1;
//                             bfs_flag = 1;
//                         }
//                     }
//                     else if(j >  0){
//                         if(bfs_nodes[i][j-1].visited && bfs_nodes[i][j-1].has_bomb == 0){
//                             bfs_nodes[i][j].parent_pos[0] = i;
//                             bfs_nodes[i][j].parent_pos[1] = j - 1;
//                             bfs_nodes[i][j].visited = 1;
//                             bfs_flag = 1;
//                         }
//                     }
//                     else if(j <  4){
//                         if(bfs_nodes[i][j+1].visited && bfs_nodes[i][j+1].has_bomb == 0){
//                             bfs_nodes[i][j].parent_pos[0] = i;
//                             bfs_nodes[i][j].parent_pos[1] = j + 1;
//                             bfs_nodes[i][j].visited = 1;
//                             bfs_flag = 1;
//                         }
//                     }
//                 }
//             }
//         }
//     }
//     printf("bfs first stage success\n");
//     if(bfs_nodes[target_x][target_y].visited){
//         int bfs_last_node[2];
//         int bfs_curr_node[2];
//         bfs_last_node[0] = target_x;
//         bfs_last_node[1] = target_y;
//         bfs_curr_node[0] = bfs_nodes[target_x][target_y].parent_pos[0];
//         bfs_curr_node[1] = bfs_nodes[target_x][target_y].parent_pos[1];
//         while(!(bfs_curr_node[0] == curr_node[0] && bfs_curr_node[1] == curr_node[1])){
//             bfs_last_node[0] = bfs_curr_node[0];
//             bfs_last_node[1] = bfs_curr_node[1];
//             bfs_curr_node[0] = bfs_nodes[bfs_last_node[0]][bfs_last_node[1]].parent_pos[0];
//             bfs_curr_node[1] = bfs_nodes[bfs_last_node[0]][bfs_last_node[1]].parent_pos[1];
//         }
//         int target_direction;
//         if(bfs_last_node[0] == curr_node[0] + 1){
//             target_direction = 1;
//         }
//         else if(bfs_last_node[0] == curr_node[0] - 1){
//             target_direction = 3;
//         }
//         else if(bfs_last_node[1] == curr_node[1] + 1){
//             target_direction = 0;
//         }
//         else if(bfs_last_node[1] == curr_node[1] - 1){
//             target_direction = 2;
//         }
//         return ((target_direction - curr_direction) % 4 == 3)? -1 : ((target_direction - curr_direction) % 4);
//     }    
//     else return -2;//not reachable!
// }

// int find_path(int target_x, int target_y){
//     printf("find_path called!\n\n");
//     if(before_node == 0){
//         switch(curr_direction){
//             case 0://east
//                 if(dgist.map[curr_node[0]][curr_node[1] + 1].item.status == 2){
//                     if(dgist.map[curr_node[0]][curr_node[1]].item.status == 2) return 2;
//                     else{
//                         turn_left(file);
//                         curr_direction = (curr_direction + 2) % 4;
//                         before_node = 1;
//                         return find_path(target_x, target_y);
//                     }
//                 }
//                 break;
//             case 1://south
//                 if(dgist.map[curr_node[0] + 1][curr_node[1]].item.status == 2){
//                     if(dgist.map[curr_node[0]][curr_node[1]].item.status == 2) return 2;
//                     else{
//                         turn_left(file);
//                         curr_direction = (curr_direction + 2) % 4;
//                         before_node = 1;
//                         return find_path(target_x, target_y);
//                     }
//                 }
//                 break;
//             case 2://west
//                 if(dgist.map[curr_node[0]][curr_node[1] - 1].item.status == 2){
//                     if(dgist.map[curr_node[0]][curr_node[1]].item.status == 2) return 2;
//                     else{
//                         turn_left(file);
//                         curr_direction = (curr_direction + 2) % 4;
//                         before_node = 1;
//                         return find_path(target_x, target_y);
//                     }
//                 }
//                 break;
//             case 3://north
//                 if(dgist.map[curr_node[0] - 1][curr_node[1]].item.status == 2){
//                     if(dgist.map[curr_node[0]][curr_node[1]].item.status == 2) return 2;
//                     else{
//                         turn_left(file);
//                         curr_direction = (curr_direction + 2) % 4;
//                         before_node = 1;
//                         return find_path(target_x, target_y);
//                     }
//                 }
//                 break;
//         }
//         return 2;
//     }
//     return bfs(target_x,target_y);
// }

// void push_target_queue(int next_x, int next_y){
//     for(int i = 0; i < 3; i ++){
//         target_node_queue[i][0] = target_node_queue[i+1][0]; 
//         target_node_queue[i][1] = target_node_queue[i+1][1];
//     }
//     target_node_queue[3][0] = next_x;
//     target_node_queue[3][1] = next_y;
// }

// void update_action(){
//     if(curr_node[0] == 1 && curr_node[1] == 2 && target_node_queue[0][0] == 1 && target_node_queue[0][1] == 2){
//         push_target_queue(0,0);
//     }
//     else if(curr_node[0] == 2 && curr_node[1] == 1 && target_node_queue[0][0] == 2 && target_node_queue[0][1] == 1){
//         push_target_queue(4,0);
//     }
//     else if(curr_node[0] == 2 && curr_node[1] == 3 && target_node_queue[0][0] == 2 && target_node_queue[0][1] == 3){
//         push_target_queue(4,4);
//     }
//     else if(curr_node[0] == 3 && curr_node[1] == 2 && target_node_queue[0][0] == 3 && target_node_queue[0][1] == 2){
//         push_target_queue(0,4);
//     }
//     else if(curr_node[0] == 0 && curr_node[1] == 0 && target_node_queue[0][0] == 0 && target_node_queue[0][1] == 0){
//         push_target_queue(4,0);
//     } 
//     else if(curr_node[0] == 4 && curr_node[1] == 0 && target_node_queue[0][0] == 4 && target_node_queue[0][1] == 0){
//         push_target_queue(4,4);
//     }
//     else if(curr_node[0] == 4 && curr_node[1] == 4 && target_node_queue[0][0] == 4 && target_node_queue[0][1] == 4){
//         push_target_queue(0,4);
//     }
//     else if(curr_node[0] == 0 && curr_node[1] == 4 && target_node_queue[0][0] == 0 && target_node_queue[0][1] == 4){
//         push_target_queue(0,0);
//     }
//     next_action = -2;
//     while(next_action == -2){
//         next_action = find_path(target_node_queue[0][0],target_node_queue[0][1]);
//     }
// }

void* read_qr(void * arg){
    cv::VideoCapture cap(0);
    cv::QRCodeDetector qrDecoder;
    cv::Mat frame;
    cv::Mat brightenedImage;
    std::vector <cv::Point> points;
    std::string qrCodeText;
    while(1){
        if (!cap.isOpened()) {  // Check if camera opened successfully
            cv::VideoCapture temp_cap(0);
            cap = temp_cap;
        }
        qrCodeText = "";
        cap >> frame;
        qrCodeText = qrDecoder.detectAndDecode(frame, points);
        if (qrCodeText.size() > 0){
            qr = std::stoi(qrCodeText);
            qr_changed = 1;
        }
    }
}

void* receiveData(void* arg) {
    while (1) {
        int valRead = read(sock, &dgist, sizeof(DGIST));
        if (valRead >= 0) {
            dgist_refreshed = 1;
        }
        pthread_mutex_lock(&lock);
        // // Print map and player information
         printf("========== PRINT MAP ==========\n");
         for (int i = 0; i < MAP_ROW; i++) {
             for (int j = 0; j < MAP_COL; j++) {
                 Item tmpItem = dgist.map[i][j].item;
                 switch (tmpItem.status) {
                     case nothing:
                         printf("- ");
                         break;
                     case item:
                         printf("%d ", tmpItem.score);
                         break;
                     case trap:
                         printf("x ");
                         break;
                 }
             }
             printf("\n");
         }
         printf("========== PRINT DONE ==========\n");

         printf("========== PRINT PLAYERS ==========\n");
         for (int i = 0; i < MAX_CLIENTS; i++) {
             client_info client = dgist.players[i];
             printf("++++++++++ Player %d ++++++++++\n", i + 1);
             printf("Location: (%d, %d)\n", client.row, client.col);
             printf("Score: %d\n", client.score);
             printf("Bomb: %d\n", client.bomb);
         }
         printf("========== PRINT DONE ==========\n");
    }
    return NULL;
}
void* sendAction(void* arg) {
    cAction_info* cA_info = (cAction_info*)arg;
    while (1) {
        if(send_data_flag){
            ClientAction cAction;
            cAction.row = action[0];
            cAction.col = action[1];
            cAction.action = (enum Action)action[2];
            pthread_mutex_lock(&lock);
            if (send(sock, &cAction, sizeof(ClientAction), 0) < 0){
                printf("Send Error");

            }
            else{
                send_data_flag = 0;
                printf("sent data row: %d, col: %d, action: %d\n",cAction.row, cAction.col, cAction.action);
            }
            pthread_mutex_unlock(&lock);
            printf("unlock success!");
        // int row, col, action;
        // printf("Enter your action (row col action): ");
        // scanf("%d %d %d", &row, &col, &action);

        // ClientAction cAction;
        // cAction.row = row;
        // cAction.col = col;
        // cAction.action = (enum Action)action;
        
        //printf("pthread_mutex_lock - before");
        //pthread_mutex_lock(&lock);
        //printf("pthread_mutex_lock - after");
        //if (send(sock, &cAction, sizeof(ClientAction), 0) < 0) {
        // if(*(cA_info->dirty_pointer)){
        //     if (send(sock, cA_info->cAction_point, sizeof(ClientAction), 0) <=0) {
        //         perror("Send error");
        //         exit(EXIT_FAILURE);
        //     }
        // //printf("pthread_mutex_unlock - before");
        // }
        //printf("%d %d %d\n", cA_info->cAction_point->row, cA_info->cAction_point->col, cA_info->cAction_point->action);
        //printf("pthread_mutex_unlock - after");
            delay(10);
        }
    }
    return NULL;
}
void* networking(void* arg) {
    //if (argc != 3) {
    //    fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
    //    return 1;
    //}
    networking_info* ninfo = (networking_info*)arg;
    char* server_port_str = ninfo->server_port_str;
    char* server_ip = ninfo->server_ip;
    cAction_info* cA_info = ninfo->cA_info;

    //const char *server_ip = argv[1];
    int server_port = atoi(server_port_str);
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return NULL;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);

    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return NULL;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return NULL;
    }

    printf("Connected to server at %s:%d\n", server_ip, server_port);
    

    pthread_t recvThread, sendThread;
    pthread_mutex_init(&lock, NULL);

    // Create threads for receiving and sending data
    pthread_create(&recvThread, NULL, receiveData, NULL);
    pthread_create(&sendThread, NULL, sendAction, (void*)cA_info);

    // Join threads
    pthread_join(recvThread, NULL);
    pthread_join(sendThread, NULL);

    close(sock);
    pthread_mutex_destroy(&lock);
    return NULL;
}

void adjust_left(int file_dir){
    while(!(leftin == LOW || rightin == LOW)){
        flag = controlMotors(file_dir, 0, 0, 1, 70);
        leftout = digitalRead(LEFT1);
        leftin = digitalRead(LEFT2);
        rightin = digitalRead(RIGHT1);
        rightout = digitalRead(RIGHT2);
        delay(10);
    }
    flag = 0;
}
void adjust_right(int file_dir){
    while(!(leftin == LOW || rightin == LOW)){
        controlMotors(file_dir, 1, 70, 0, 0);
        leftout = digitalRead(LEFT1);
        leftin = digitalRead(LEFT2);
        rightin = digitalRead(RIGHT1);
        rightout = digitalRead(RIGHT2);
        delay(10);
    }
    flag = 0;
}

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("correct usage: <executable name> ip_address port");
    }
    cAction_info cActionInf;
    ClientAction cp; 
    networking_info* nInf;
    int dpoint;
    networking_info nInfContain;
    pthread_t networkThread;
    pthread_t qrThread;
    cActionInf.cAction_point = &cp;
    cActionInf.dirty_pointer = &dpoint;
    nInf = &nInfContain;
    
    printf("%s\n",argv[1]);
    printf("%s\n",argv[2]);
    
    nInf->server_ip = argv[1];
    nInf->server_port_str = argv[2];
    nInf->cA_info = &cActionInf;
    
    pthread_create(&networkThread, NULL, networking, (void*)nInf);
    pthread_create(&qrThread, NULL, read_qr, NULL);
    const char *filename = "/dev/i2c-1";
    // Open the I2C bus
    if ((file = open(filename, O_RDWR)) < 0) {
        perror("Failed to open the i2c bus\n");
        return 1;
    }
    // Specify the I2C slave address
    if (ioctl(file, I2C_SLAVE, I2C_ADDR) < 0) {
        perror("Failed to acquire bus access and/or talk to slave");
        close(file);
        return 1;
    }

    // Initialize GPIO pins
    setup();
    leftout = digitalRead(LEFT1);
    leftin = digitalRead(LEFT2);
    rightin = digitalRead(RIGHT1);
    rightout = digitalRead(RIGHT2);
    //set_communication()
    int temp_x;
    int temp_y;
    int player_num = -1;
    queue_start = NULL;
    queue_append(0);
    int init_path_flag = 0;
    while(1){
        //printf("curr_position:%d/%d, curr_direction:%d, before_node: %d\n", curr_node[0],curr_node[1], curr_direction, before_node);
        if(qr_changed){
            qr_changed = 0;
            printf("QR success!\n");
            printf("%d\n",qr);
            printf("==========================\n");
            printf("0\n");
            temp_x = qr / 10;
            temp_y = qr % 10;
            if(init_path_flag == 0){
                init_path_flag = 1;
                if(temp_x == 0 || temp_y == 0){
                    queue_append(1);
                    queue_append(0);
                    queue_append(0);
                    queue_append(-1);
                    queue_append(0);
                    queue_append(-1);
                    queue_append(0);
                    queue_append(0);
                    queue_append(1);
                }   
                else if(temp_x == 4 || temp_y == 4){
                    queue_append(-1);
                    queue_append(0);
                    queue_append(0);
                    queue_append(1);
                    queue_append(0);
                    queue_append(1);
                    queue_append(0);
                    queue_append(0);
                    queue_append(-1);
                }   
            }
            if(temp_x != curr_node[0] || temp_y != curr_node[1]){
                printf("reached new node!\n");
                curr_node[0] = temp_x;
                curr_node[1] = temp_y;
                //get new direction
                if((temp_x == 1 && temp_y == 1) || (temp_x == 1 && temp_y == 3) || (temp_x == 3 && temp_y == 1) || (temp_x == 3 && temp_y == 3)){
                    send_data(temp_x, temp_y,1);
                }
                else send_data(temp_x, temp_y, 0);
            }
        }
        leftout = digitalRead(LEFT1);
        leftin = digitalRead(LEFT2);
        rightin = digitalRead(RIGHT1);
        rightout = digitalRead(RIGHT2);

        if(is_intersection(leftout, leftin, rightin, rightout)){
            next_action = queue_pop();
            printf("attempting action %d", next_action);
            switch(next_action){
                case -1:
                    turn_left(file);
                    curr_direction = (curr_direction - 1) / 4;
                    break;
                case 0:
                    go_straight(file);
                    break;
                case 1:
                    turn_right(file);
                    curr_direction = (curr_direction + 1) / 4;
                    break;
                case 2:
                    turn_left(file);
                    turn_left(file);
                    curr_direction = (curr_direction + 2) / 4;
            }
        }
        else if (leftin == HIGH && rightin == LOW){
            controlMotors(file,1,70,1,0);
        }
        else if (leftin == LOW && rightin == HIGH){
            controlMotors(file,1,0,1,70);
        }
        else if (leftout == LOW) {
            adjust_left(file);
        } else if (rightout == LOW) {
            adjust_right(file);
        } else {
            controlMotors(file,1,70,1,70);
        } 
        delay(10);
    }
    
}
