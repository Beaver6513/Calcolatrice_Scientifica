from gdb.printing import PrettyPrinter, register_pretty_printer
import gdb

class ListPrettyPrinter(object):
    """Print 'struct string' as a sequence"""

    def __init__(self, val):
        self.val = val

    def to_string(self):
        char = self.val["character"]
        return chr(char)



class CustomPrettyPrinterLocator(PrettyPrinter):
    """Given a gdb.Value, search for a custom pretty printer"""

    def __init__(self):
        super(CustomPrettyPrinterLocator, self).__init__("my_pretty_printers", [])

    def __call__(self, val):
        """Return the custom formatter if the type can be handled"""

        typename = gdb.types.get_basic_type(val.type).tag
        if typename is None:
            typename = val.type.name

        if typename == "c_node":
            return ListPrettyPrinter(val)
        


class ListDumpCmd(gdb.Command):
    """Prints the ListNode from our example in a nice format!"""

    def __init__(self):
        super(ListDumpCmd, self).__init__("plist", gdb.COMMAND_USER)

    def _list_to_str(self, val):
        """Walk through the c_node list.

        We will simply follow the 'next' pointers until we encounter NULL
        """
        node_ptr = val
        result = ""
        while node_ptr != 0:
            char = node_ptr["character"]
            result += chr(char)
            node_ptr = node_ptr["next"]
        return result

    def complete(self, text, word):
        # We expect the argument passed to be a symbol so fallback to the
        # internal tab-completion handler for symbols
        return gdb.COMPLETE_SYMBOL

    def invoke(self, args, from_tty):
        # We can pass args here and use Python CLI utilities like argparse
        # to do argument parsing

        node_ptr_val = gdb.parse_and_eval(args)
        if str(node_ptr_val.type) != "c_node *":
            print("Expected pointer argument of type (c_node)")
            print(str(node_ptr_val.type))
            return

        print(self._list_to_str(node_ptr_val))



register_pretty_printer(None, CustomPrettyPrinterLocator(), replace=True)
ListDumpCmd()