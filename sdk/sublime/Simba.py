import subprocess
import os
import sublime, sublime_plugin

global_name = None

def new(view, subcommand):

    def on_path_done(path):
        current_file_dir = os.path.dirname(view.file_name())
        app_dir = os.path.join(current_file_dir, path)
            
        try:
            subprocess.check_call(["simba", subcommand, "--create", global_name],
                                  cwd=app_dir)
        except OSError as e:
            sublime.message_dialog(app_dir + ": " + e.strerror)
            raise
            
        sublime.message_dialog("Successfully created '{name}'.".format(name=global_name))

    def on_done(name):
        global global_name
        global_name = name
        sublime.Window.show_input_panel(sublime.active_window(),
                                        "Path",
                                        "",
                                        on_path_done,
                                        None,
                                        None)

    sublime.Window.show_input_panel(sublime.active_window(),
                                    "Name",
                                    "",
                                    on_done,
                                    None,
                                    None)


class SimbaApplicationNewCommand(sublime_plugin.TextCommand):

    def run(self, _):
        new(self.view, "application")


class SimbaPackageNewCommand(sublime_plugin.TextCommand):

    def run(self, _):
        new(self.view, "package")


class SimbaPackageReleaseCommand(sublime_plugin.TextCommand):

    def run(self, _):
        package_dir = os.path.dirname(self.view.file_name())
        try:
            subprocess.check_call(["simba", "package", "--release"],
                                  cwd=package_dir)
        except OSError as e:
            sublime.message_dialog(app_dir + ": " + e.strerror)
            raise
        
        sublime.message_dialog("Successfully released package.")
           

class SimbaPackageInstallCommand(sublime_plugin.TextCommand):
    def run(self, _):
        print "Not implemented"
