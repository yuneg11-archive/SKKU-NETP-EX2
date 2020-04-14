waf_path="../waf"
export "NS_LOG=UdpEchoServerApplication=level_info|prefix_time|prefix_node|prefix_func:UdpEchoClientApplication=level_info|prefix_time|prefix_node|prefix_func"
${waf_path} --run Exercise2 > Exercise2Log.out 2>&1