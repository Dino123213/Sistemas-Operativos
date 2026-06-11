struct msg_buffer {
    long mtype;          // obligatorio, tipo del mensaje
    char mtext[100];     // contenido del mensaje
};

key_t key = ftok("progfile", 65);
int msgid = msgget(key, 0666 | IPC_CREAT);

struct msg_buffer message;
message.mtype = 1;
strcpy(message.mtext, "Hola desde el padre!");
msgsnd(msgid, &message, sizeof(message.mtext), 0);