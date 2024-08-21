/*The current code does not use encryption.

In ESP-NOW, encryption is optional and must be explicitly enabled. Since the code you’re using defines and registers peers with esp_now_add_peer without specifying an encryption key, it is operating in unencrypted mode.

How to Check:
Unencrypted Mode: If the esp_now_peer_info_t struct used in esp_now_add_peer does not have an encryption key set (i.e., the peer_encryption field is not being configured), the communication is unencrypted.
Encrypted Mode: If you want to use encryption, you need to set the lmk (Local Master Key) field in esp_now_peer_info_t and ensure encrypt is set to true.
Since the code doesn't include setting any encryption keys, it's currently configured for unencrypted communication, which allows for up to 20 peers.*/
