
VMeter_v0ps.dll: dlldata.obj VMeter_v0_p.obj VMeter_v0_i.obj
	link /dll /out:VMeter_v0ps.dll /def:VMeter_v0ps.def /entry:DllMain dlldata.obj VMeter_v0_p.obj VMeter_v0_i.obj \
		kernel32.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DREGISTER_PROXY_DLL \
		$<

clean:
	@del VMeter_v0ps.dll
	@del VMeter_v0ps.lib
	@del VMeter_v0ps.exp
	@del dlldata.obj
	@del VMeter_v0_p.obj
	@del VMeter_v0_i.obj
