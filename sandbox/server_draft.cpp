


int main()
{
    socket();
    setsockopt();
    fcntl();
    bind();
    listen();
    add_to_kq.add(sock);
    kque();
    while(1)
    {
        kque.wait();
        kque.chec();
        if (receve())
        {
            parse();
        }
        else if (open_wirte)
        {
            response.send()
        }
        if (!fd_inuse())
            close_and_remove_from_kqueue(); 
    }

}