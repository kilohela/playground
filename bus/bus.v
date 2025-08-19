`timescale 1ns/1ps

module key_scanner (
    input wire clk,
    input wire rst_n,

    output reg valid,
    input wire ready,
    output reg [7:0] data_out
);

    reg [3:0] cnt;
    reg start;

    // 模拟按键事件：在第5个周期产生一个呼吸灯命令
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            cnt <= 0;
            start <= 0;
        end else begin
            cnt <= cnt + 1;
            if (cnt == 5) start <= 1;  // 模拟按键按下
        end
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            valid <= 0;
            data_out <= 0;
        end else begin
            // if (valid && ready) : 该次传输有效（在该次时钟沿发生），准备下一次传输
            // if (!valid) : 该次传输无效，准备下一次传输
            if (!valid || (valid && ready)) begin
                if (start) begin
                    data_out <= 8'hFF;  // 呼吸灯命令
                    valid    <= 1;
                    start    <= 0;     // 消费事件
                end else begin
                    valid <= 0;  // 空闲
                end
            end
            // 否则：valid=1 && ready=0 → 保持
        end
    end

endmodule

module led_controller (
    input wire clk,
    input wire rst_n,

    input wire valid,
    output reg ready,
    input wire [7:0] data_in,  // 接收命令
    output reg led
);

    reg [1:0] busy_cnt;  // 呼吸灯初始化需要3个周期

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            busy_cnt <= 0;
            ready <= 1;
        end else begin
            if (busy_cnt > 0) begin
                busy_cnt <= busy_cnt - 1;
                if (busy_cnt == 1) begin
                    ready <= 1;
                end
            end

            // 检测有效传输
            if (valid && ready) begin
                if (data_in == 8'hFF) begin
                    busy_cnt <= 2;  // 3周期处理时间（当前周期 + 2）
                    ready <= 0;  // 忙碌
                end else if (data_in == 8'h01) begin
                    led <= 1;
                end else if (data_in == 8'h02) begin
                    led <= 0;
                end
            end
        end
    end

endmodule

module bus_tb;

    reg clk;
    reg rst_n;

    wire valid;
    wire ready;
    wire [7:0] data;
    wire led;

    key_scanner ks (
        .clk(clk),
        .rst_n(rst_n),
        .valid(valid),
        .ready(ready),
        .data_out(data)
    ); 

    led_controller lc (
        .clk(clk),
        .rst_n(rst_n),
        .valid(valid),
        .ready(ready),
        .data_in(data),
        .led(led)
    );

    always #5 clk = ~clk;  // 时钟周期为10ns

    initial begin
        $dumpfile("bus.vcd");
        $dumpvars(0, bus_tb);
        clk = 0;
        rst_n = 0;
        #5 rst_n = 1;  // 复位信号
        #200;
        $finish;  
    end

endmodule

