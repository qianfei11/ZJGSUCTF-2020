#!/usr/bin/env python
import re

with open('code.txt', 'rb') as f:
    code = f.read()
pat = re.compile(r'scancode = .{3,4},')
code = pat.findall(code)
scancodes = [int(c[11:-1], 16) for c in code]
mappings = {
    0x00: 'Error', 0x01: 'Esc', 0x02: '1!', 0x03: '2@', 0x04: '3#', 0x05: '4$', 0x06: '5%', 0x07: '6^', 0x08: '7&', 0x09: '8*', 0x0a: '9(', 0x0b: '0)', 0x0c: '-_', 0x0d: '=+', 0x0e: 'Backspace',
    0x0f: 'Tab', 0x10: 'q', 0x11: 'w', 0x12: 'e', 0x13: 'r', 0x14: 't', 0x15: 'y', 0x16: 'u', 0x17: 'i', 0x18: 'o', 0x19: 'p', 0x1a: '[{', 0x1b: ']}', 0x1c: 'Enter',
    0x1d: 'LCtrl', 0x1e: 'a', 0x1f: 's', 0x20: 'd', 0x21: 'f', 0x22: 'g', 0x23: 'h', 0x24: 'j', 0x25: 'k', 0x26: 'l', 0x27: ';:', 0x28: '\'"', 0x29: '`~',
    0x2c: 'z', 0x2d: 'x', 0x2e: 'c', 0x2f: 'v', 0x30: 'b', 0x31: 'n', 0x32: 'm', 0x33: ',<', 0x34: '.>', 0x35: '/?', 0x36: 'RShift', 0x37: 'Keypad-*/PrtScn',
    0x38: 'LAlt', 0x39: 'Space bar', 0x3a: 'CapsLock',
    0x3b: 'F1', 0x3c: 'F2', 0x3d: 'F3', 0x3e: 'F4', 0x3f: 'F5', 0x40: 'F6', 0x41: 'F7', 0x42: 'F8', 0x43: 'F9', 0x44: 'F10',
    0x45: 'NumLock', 0x46: 'ScrollLock',
    0x47: 'Keypad-7/Home', 0x48: 'Keypad-8/Up', 0x49: 'Keypad-9/PgUp', 0x4a: 'Keypad--', 0x4b: 'Keypad-4/Left', 0x4c: 'Keypad-5', 0x4d: 'Keypad-6/Right', 0x4e: 'Keypad-+', 0x4f: 'Keypad-1/End', 0x50: 'Keypad-2/Down', 0x51: 'Keypad-3/PgDn', 0x52: 'Keypad-0/Ins', 0x53: 'Keypad-./Del'
}
flag = ''
for c in scancodes:
    if c == 0xe0:
        #print '[*] Keypad Press'
        pass
    elif c in mappings.keys():
        if mappings[c] == 'Backspace':
            flag = flag[:-1]
        elif mappings[c] == ']}':
            flag += '}'
        elif mappings[c] == '[{':
            flag += '{'
        elif mappings[c] == '-_':
            flag += '_'
        else:
            flag += mappings[c][0]
        print hex(c), mappings[c]
    elif c - 0x80 in mappings.keys():
        #print hex(c), 'release', mappings[c - 0x80]
        pass
print flag

