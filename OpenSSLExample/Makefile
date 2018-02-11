SSL_EXAMPLE_DIRS    = ./SSL_Example
CRYPTO_EXAMPLE_DIRS = ./CRYPTO_Example

CP = cp
CRT_FILE = ./KeyFile/server.crt
KEY_FILE = ./KeyFile/server.key

LOCAL_CRT_FILE = my.crt
LOCAL_KEY_FILE = my.key

SSL_EXAMPLE_CRT_FILE = ./SSL_Example/$(LOCAL_CRT_FILE)
SSL_EXAMPLE_KEY_FILE = ./SSL_Example/$(LOCAL_KEY_FILE)


all: ssl crypto
	$(CP) $(CRT_FILE) $(SSL_EXAMPLE_CRT_FILE)
	$(CP) $(KEY_FILE) $(SSL_EXAMPLE_KEY_FILE)

	$(CP) $(CRT_FILE) $(LOCAL_CRT_FILE)
	$(CP) $(KEY_FILE) $(LOCAL_KEY_FILE)

ssl:
	$(MAKE) -C $(SSL_EXAMPLE_DIRS) all
	
crypto:
	$(MAKE) -C $(CRYPTO_EXAMPLE_DIRS) all

clean:
	rm -rf $(SSL_EXAMPLE_CRT_FILE)
	rm -rf $(SSL_EXAMPLE_KEY_FILE)
	$(MAKE) -C $(SSL_EXAMPLE_DIRS) clean
	$(MAKE) -C $(CRYPTO_EXAMPLE_DIRS) clean

