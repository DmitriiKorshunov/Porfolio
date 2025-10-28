import contextlib, datetime, threading, uvicorn, json, time

hour = datetime.datetime.today().hour
minute = datetime.datetime.today().minute
settings = json.loads(open("config_files/settings.json", "r").read())
reboot = False


class Server(uvicorn.Server):
    def install_signal_handlers(self):
        pass

    @contextlib.contextmanager
    def run_in_thread(self):
        thread = threading.Thread(target=self.run)
        thread.start()
        try:
            while not self.started:
                time.sleep(1e-3)
                yield
        finally:
            self.should_exit = True
            thread.join()


def reboot_docs():
    global reboot
    config1 = uvicorn.Config("main:app", host=settings['host'], port=settings['port_docs'], log_config=None)
    server1 = Server(config=config1)
    with server1.run_in_thread():
        print(f'{datetime.datetime.today()} Сервер документооборота запущен')
        while not reboot:
            time.sleep(0.1)
        print(f'{datetime.datetime.today()} Сервер документооборота остановлен')


def reboot_chats():
    global reboot
    print(f'{datetime.datetime.today()} Сервер чатов запущен')
    config2 = uvicorn.Config("chats:app", host=settings['host'], port=settings['port_chat'], log_config=None)
    server2 = Server(config=config2)
    with server2.run_in_thread():
        while not reboot:
            time.sleep(0.1)
        print(f'{datetime.datetime.today()} Сервер чатов остановлен')


def reboot_time():
        global reboot
        while not reboot:
            if datetime.datetime.today().hour == settings['auto_reboot_time']['hour']:
                if datetime.datetime.today().minute == settings['auto_reboot_time']['minute']:
                    if datetime.datetime.today().second < 10:
                        reboot = True
        print(f'{datetime.datetime.today()} Запуск автоматической перезагрузки')
        time.sleep(10)
        reboot = False


while True:
    try:
        thread_chats = threading.Thread(target=reboot_chats)
        thread_docs = threading.Thread(target=reboot_docs)
        thread_time = threading.Thread(target=reboot_time)
        thread_chats.start()
        time.sleep(0.1)
        thread_docs.start()
        time.sleep(0.1)
        thread_time.start()
        time.sleep(0.1)
        thread_docs.join()
        thread_chats.join()
        thread_time.join()
    except:
        pass
