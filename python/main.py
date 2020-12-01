import extension_call_python_callback
import asyncio


def callback(x):
    print(f"Python callback: {x}")


if __name__ == '__main__':
    cls = extension_call_python_callback.ExtensionClass(callback)
    cls.start_background_thread()
    loop = asyncio.new_event_loop()

    try:
        loop.run_forever()
    except KeyboardInterrupt:
        print("Finalizing in python")
