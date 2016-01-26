var HashMap = require('hashmap')
var xmpp = require('node-xmpp-server')
var server = null
var clients = new HashMap()

// Sets up the server.
server = new xmpp.C2S.TCPServer({
	autostart: false,
	port: process.env.PORT || 5222
})

// On connection event. When a client connects.
server.on('connection', function (client) {
	// That's the way you add mods to a given server.
	console.log('connection')
	// Allows the developer to register the jid against anything they want
	client.on('register', function (opts, cb) {		
		console.log('REGISTER', client.jid)
	})

	// Allows the developer to authenticate users against anything they want.
	client.on('authenticate', function (opts, cb) {
		console.log('server:', opts, 'AUTHENTICATING')
		var full = opts.jid._local + '@' + opts.jid._domain
		if (!clients.has(full))	{
			clients.set(full, {client:client, messages:[]})
			cb(null, opts)
		} else if(clients.get(full).client == null) {
			clients.get(full).client = client
			cb(null, opts)
						
		} else {
			console.log('server:', opts.username, 'AUTH FAIL')
			cb(false)
		}
	})

	client.on('online', function () {
		console.log('server:', client.jid.local, 'ONLINE')
		var full = client.jid._local + '@' + client.jid._domain
		if(clients.get(full).messages.length > 0)
		{
			clients.get(full).messages.forEach(function(item, i, arr) {
				handleMessage(item)
			})
			clients.get(full).messages = []
		}
	})

	// Stanza handling
	client.on('stanza', function (stanza) {
		console.log('server:', client.jid.local, 'stanza', stanza.toString())		
		if (stanza.is('iq') && stanza.attrs.id && stanza.attrs.id == 'get-active-users') {
			handleActiveUserList(stanza, client)
		} else if (stanza.is('message')) {
			handleMessage(stanza)
		} else if (stanza.is('iq') && stanza.attrs.id && stanza.attrs.id == 'public-key') {
			handlePublicKey(stanza)
		}
	})

	// On Disconnect event. When a client disconnects
	client.on('disconnect', function () {
		console.log('server:', 'DISCONNECT')
		if (client.jid != null && client.jid.local != null) {
			var full = client.jid._local + '@' + client.jid._domain
			console.log('server:', 'DISCONNECT', full)
			if (clients.has(full))
			{
				clients.get(full).client = null;
			}
		}
	})
})

/* request
<iq id="get-active-users" 
	type="set" 
	to="test@localhost" 
	xmlns:stream="http://etherx.jabber.org/streams" 
	from="test@localhost/097995cb4307d0462ea2ccb0e1813aa3"><command/></iq>
*/
/* response
<iq from='shakespeare.lit'
    id='get-active-users'
    to='bard@shakespeare.lit/globe'
    type='result'
    xml:lang='en'>
  <command status='completed'>
    <x xmlns='jabber:x:data' type='result'>
      <field label='The list of active users'
             var='activeuserjids'>
        <value jid="bard@shakespeare.lit" key="KEYDATABASE64"></value>
        <value jid="crone1@shakespeare.lit" key="KEYDATABASE64"></value>
      </field>
    </x>
  </command>
</iq>
*/
function handleActiveUserList(stanza, client) {
	var field = new xmpp.IQ({ id:'get-active-users', type:'set', to: stanza.from }).
		c('query', {label:'The list of active users'})
	clients.forEach(function(value, key) {
		field.c('value', {jid: key, key: value.key})
	});
	client.send(field.root())
}

/*
<message to="asd@localhost" 
	xmlns:stream="http://etherx.jabber.org/streams" 
	from="test@localhost/272f27274b5c491dac482ccab0da0035">
	<body>message text</body>
</message>
*/
function handleMessage(stanza)
{
	if (stanza.to != null)
	{
		if (!clients.has(stanza.to))
		{
			clients.set(stanza.to, {client:null, messages:[]})
		}
		else if (clients.get(stanza.to).client != null)
		{
			clients.get(stanza.to).client.send(stanza)
		}
		else
		{
			clients.get(stanza.to).messages.push(stanza)
		}
	}	
}

/*
<iq id="public-key"  
	xmlns:stream="http://etherx.jabber.org/streams" 
	from="test@localhost/272f27274b5c491dac482ccab0da0035"
	key="KEYDATABASE64">	
</iq>
*/
function handlePublicKey(stanza)
{
	if (stanza.from != null)
	{
		var from = stanza.attrs.from.substr(0, stanza.attrs.from.indexOf('/')) 
		if (!clients.has(from))
		{
			clients.set(from, {client:null, messages:[], key: stanza.attrs.key})
		}
		else
		{
			clients.get(from).key = stanza.attrs.key
		}
	}
}

server.on('online', function () {
	console.log('server online')
})

server.on('shutdown', function () {
	console.log('server shutdown')
})

server.listen()
