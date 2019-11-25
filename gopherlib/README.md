Gopherlib
===
Gopherlib is VexU team UMN's common code repository for PROS.

Installing
===
```
git clone https://gitlab.com/vexu-umn/gopherlib.git
prosv5 conduct apply --force-apply okapilib
prosv5 conduct apply --force-apply kernel
make template
prosv5 conduct fetch gopherlib@xxxx.a.b-c.d.zip
```