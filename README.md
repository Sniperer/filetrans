## filetrans
用来支持不同主机间文件互传。采用C/S模式，服务器端一般假设在linux中。
<br>&emsp;&emsp;——致力为某电某无意义的b级测试提供解决方案；并缅怀我迟到10分钟的a级测试。
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
### ``filetrans/cilent/linux``<br>
为linux客户端所设计的源代码文件夹。

### ``filetrans/cilent/windows``<br>
为windows客户端设计的源代码文件夹。

