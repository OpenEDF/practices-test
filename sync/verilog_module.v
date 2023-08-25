
if (ctrl_multi_div && ~ready)
    stall '= 1;
else
    stall '= 0;


ctrl_multi_div: from if_id
multi_ready:    from multi module
div_ready  :    from div module

as ready = multi_ready_w | div_ready_w;
as multi_ready = multi_ready_w;
as div_ready = div_ready_w;

:q
