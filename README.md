## filetrans
用来支持不同主机间文件互传。采用C/S模式，服务器端一般假设在linux中。
<br>&emsp;&emsp;——致力为某电某无意义的b级测试提供解决方案；并缅怀我迟到10分钟的a级测试。

## 编译和使用

`服务端` <br>
<pre>
cd filetrans/server
make clean&&make
./s_filetrans
</pre>


`客户端for linux` <br>
<pre>
cd filetrans/cilent/linux
make clean&&make
./c_filetrans "file path in cilent" "excepted file name in server"
</pre>

## 源代码组织架构

### ``filetrans/server`` <br>
服务端代码所在文件夹。 <br>

`服务端版本：1.0 for b-test` <br>
&emsp;特性：<br>
&emsp;&emsp;1）程序的入口在s_main.cpp中，不接受参数 <br>
&emsp;&emsp;2）代码文件中以s_socks开头的源文件都与linux系统socket编程有关。 <br>
&emsp;&emsp;3) threadpool为设计的c++线程库，将非主线程的工作，加入到线程池中。<br>
&emsp;&emsp;4）s_recv_file封装了从socket缓冲区读取文件并在本机创建文件的操作。<br>

&emsp;待加入的特性： <br>
&emsp;&emsp;1）threadpool中线程任务的优先级，并由此为不同的任务设计优先级，以进行调度。 <br>
&emsp;&emsp;2) s_recv_file中按4096个字节分别装填进本机文件，需要在文件未读取完毕前，添加文件锁。<br>
&emsp;&emsp;3) 改善硬编码的IP地址和服务端口。<br>

### ``filetrans/cilent/linux``<br>
为linux客户端所设计的源代码文件夹。<br>

`客户端版本：1.0 for b-test` <br>
&emsp;特性：<br>
&emsp;&emsp;1）程序的入口在c_main.cpp中，接受参数，且每个参数均为要传递的文件路径。 <br>
&emsp;&emsp;2）代码文件中以s_socks开头的源文件都与linux系统socket编程有关。 <br>
&emsp;&emsp;3) 简单的单线程程序，对每个要传递文件，序列化发送。<br>
&emsp;&emsp;4）c_send_file封装了从本机读取文件并在发送至目标主机socket缓冲区的操作。<br>

&emsp;待加入的特性： <br>
&emsp;&emsp;1）令人头疼的难看的图形界面。 <br>
&emsp;&emsp;2) 由于linux的RCU功能，为客户端文件添加文件锁或许有些画蛇添足。<br>
&emsp;&emsp;3) 需要更优雅的debug方案，对于无需GDB的基本逻辑bug，需要设计功能较为完善的debug宏。或许nginx的debuginfo可以借鉴。<br>
&emsp;&emsp;4) 急需加入异常处理机制，增加代码鲁棒性。 <br>

### ``filetrans/cilent/windows``<br>
为windows客户端设计的源代码文件夹。

