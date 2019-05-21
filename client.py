import binascii
import socket


def send_udp_message(message, address, port):
    message = message.replace(' ', '').replace('\n', '')
    server_address = (address, port)

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        sock.sendto(binascii.unhexlify(message), server_address)
        data, _ = sock.recvfrom(4096)
    finally:
        sock.close()
    return binascii.hexlify(data).decode('utf-8')


def format_hex(hex):
    octets = [hex[i:i+2] for i in range(0, len(hex), 2)]
    pairs = [' '.join(octets[i:i+2]) for i in range(0, len(octets), 2)]
    return '\n'.join(pairs)


message = (
    'AA AA 01 00 00 01 00 00 00 00 00 00 '
    '04 6c 69 68 7a 05 6c 6f 63 61 6c 00 00 01 00 01'
)

response = send_udp_message(message, '224.0.0.251', 5353)
print(format_hex(response))
