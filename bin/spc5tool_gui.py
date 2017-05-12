#!/usr/bin/env python3

from __future__ import print_function

import os
import subprocess
import json

try:
    from tkinter import *
    import tkinter.filedialog as filedialog
    import tkinter.messagebox as messagebox
    from tkinter.ttk import *
except ImportError:
    from Tkinter import *
    import tkFileDialog as filedialog
    import tkMessageBox as messagebox
    from ttk import *


SCRIPT_PATH = os.path.dirname(os.path.realpath(__file__))
SPC5TOOL_PY = os.path.join(SCRIPT_PATH, 'spc5tool.py')
CONFIG_JSON = 'spc5tool_gui.json'


def main():
    if os.path.isfile(CONFIG_JSON):
        with open(CONFIG_JSON) as fin:
            config = json.load(fin)
    else:
        # Default configuration.
        config = {
            'hexfile': '',
            'serial-port': 'COM1',
            'control-port': 'COM2'
        }

    root = Tk()
    root.title('SPC5 Tool GUI')

    def on_close():
        with open(CONFIG_JSON, 'w') as fout:
            json.dump({
                'hexfile': hexfile.get(),
                'serial-port': serial_port.get(),
                'control-port': control_port.get()
            }, fout, indent=4)

        root.destroy()

    root.protocol("WM_DELETE_WINDOW", on_close)

    # Emit a check event periodically every 500 ms to quit when Ctrl-C
    # is pressed in the shell.
    def check():
        root.after(500, check)

    root.after(500, check)

    input_frame = LabelFrame(root, text='Input')
    input_frame.pack(side=TOP, fill=X, padx=5, pady=5)

    # Software hexfile.
    def browse_hexfile():
        path = filedialog.askopenfilename(title='Browse')
        hexfile.set(path)

    hexfile_frame = Frame(input_frame)
    hexfile_frame.pack(side=TOP, fill=X, padx=5, pady=5)
    hexfile_label = Label(hexfile_frame, text='Software hexfile:')
    hexfile_label.pack(side=LEFT, fill=BOTH)
    hexfile = StringVar()
    hexfile.set(config['hexfile'])
    hexfile_entry = Entry(hexfile_frame, textvariable=hexfile)
    hexfile_entry.pack(side=LEFT, fill=BOTH, expand=True, padx=5, pady=5)
    hexfile_browse_button = Button(hexfile_frame, text='Browse', command=browse_hexfile)
    hexfile_browse_button.pack(side=RIGHT, padx=5, pady=5)

    serial_port_frame = Frame(input_frame)
    serial_port_frame.pack(side=TOP, fill=X)
    serial_port_label = Label(serial_port_frame, text='Serial port:')
    serial_port_label.pack(side=LEFT, fill=BOTH, padx=5, pady=5)
    serial_port = StringVar()
    serial_port.set(config['serial-port'])
    serial_port_entry = Entry(serial_port_frame, textvariable=serial_port)
    serial_port_entry.pack(side=RIGHT, fill=BOTH, expand=True, padx=5, pady=5)

    control_port_frame = Frame(input_frame)
    control_port_frame.pack(side=TOP, fill=X)
    control_port_label = Label(control_port_frame, text='Control port:')
    control_port_label.pack(side=LEFT, fill=BOTH, padx=5, pady=5)
    control_port = StringVar()
    control_port.set(config['control-port'])
    control_port_entry = Entry(control_port_frame, textvariable=control_port)
    control_port_entry.pack(side=RIGHT, fill=BOTH, expand=True, padx=5, pady=5)

    # Upload button.
    def upload():
        if not os.path.isfile(hexfile.get()):
            messagebox.showwarning(
                "Input missing",
                "Software hexfile '%s' does not exist." % hexfile.get())
            return

        if not serial_port.get():
            messagebox.showwarning(
                "Input missing",
                "Please select a serial port.")
            return

        command = ['python', '-u', SPC5TOOL_PY]
        command += ['--port', serial_port.get()]

        if control_port.get():
            command += ['--control-port', control_port.get()]

        command += ['flash_write', '--erase', '--verify', hexfile.get()]

        text.insert(END, 33 * '=' + ' UPLOAD BEGIN ' + 33 * '=' + '\n')
        text.insert(END, 'Running: {}\n'.format(' '.join(command)))
        text.insert(END, 80 * '-' + '\n')
        text.see(END)
        text.pack(fill=BOTH, expand=YES, padx=3, pady=3)

        sp = subprocess.Popen(command,
                              stdout=subprocess.PIPE,
                              stderr=subprocess.STDOUT)

        data = b''

        for byte in iter(lambda: sp.stdout.read(1), b''):
            data += byte
            data = re.sub(b'([^\n])\r([^\n])', b'\\1\n\\2', data)

            if b'\n' in data:
                line, _, data = data.partition(b'\n')
                text.insert(END, line.strip(b'\r') + b'\n')
                text.see(END)
                text.pack(fill=BOTH, expand=YES, padx=3, pady=3)
                root.update()

        exit_code = sp.wait()

        text.insert(END, 80 * '-' + '\n')

        if exit_code == 0:
            text.insert(END, "Upload successful!\n")
        else:
            text.insert(END, "Upload FAILED with {}!\n".format(exit_code))

        text.insert(END, 34 * '=' + ' UPLOAD END ' + 34 * '=' + '\n\n')
        text.see(END)
        text.pack(fill=BOTH, expand=YES, padx=3, pady=3)
        root.update()

        if exit_code == 0:
            messagebox.showinfo("Upload", "Upload successful.")
        else:
            messagebox.showerror("Upload", "Upload FAILED!")


    upload_button = Button(root, text='Upload', command=upload)
    upload_button.pack(side=TOP, padx=5, pady=5)

    output_frame = LabelFrame(root, text='Output')
    output_frame.pack(side=TOP, fill=BOTH, expand=YES, padx=5, pady=5)
    text = Text(output_frame)
    text.pack(fill=BOTH, expand=YES, padx=3, pady=3)

    root.mainloop()


if __name__ == '__main__':
    main()
