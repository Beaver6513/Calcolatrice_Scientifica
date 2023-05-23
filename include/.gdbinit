python
class LinkedListToStringPrinter:
    def __init__(self, val):
        self.val = val

    def to_string(self):
        result = ""
        node = self.val["head"]
        while node:
            result += node["character"]
            node = node["next"]
        return '"' + result + '"'

def build_pretty_printer():
    pp = gdb.printing.RegexpCollectionPrettyPrinter("LinkedListToStringPrinter")
    pp.add_printer('LinkedListToStringPrinter', '^string$', LinkedListToStringPrinter)
    return pp

gdb.printing.register_pretty_printer(gdb.current_objfile(), build_pretty_printer())
end
