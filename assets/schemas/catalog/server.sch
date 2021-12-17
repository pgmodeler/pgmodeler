# Catalog queries for server attributes
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

[SELECT name AS attribute, setting AS value FROM pg_settings WHERE name
IN (
'client_encoding',
'config_file',
'data_directory',
'dynamic_library_path',
'dynamic_shared_memory_type',
'hba_file',
'lc_collate',
'lc_ctype',
'listen_addresses',
'max_connections',
'port',
'server_encoding',
'ssl',
'ssl_ca_file',
'ssl_cert_file',
'ssl_crl_file',
'ssl_key_file',
'password_encryption',
'ident_file'
);]
