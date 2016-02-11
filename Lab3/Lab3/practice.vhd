architecture steve of shift_reg is
	signal shift_next : std_logic_vector (5 downto 0);
	
	begin
		process(clk, rst)
			if rst = '1' then
				shift <= logic_vector_std(INIT); -- assuming en is not required for rst
			else
				if (clk'event and clk = '1')
					if en = '1' then
						shift <= shift_next;
					end if;
				end if;
			end if;
		end process;
		
		shifter <= "000000" when shift = sin else
					'0' & shift(5 downto 1) when rl = '1' else --assume shift in 0, shift right
					shift(4 downto 0) when rl = '0'; --shift left
					
		rotate <= 
					shift(0) & shift(1) & shift(2) & shift(3) & shift(5) & shift(5) when rotate = '1' else-- not specific enought
					-stuff else
					
		shift_next <= 
		
	end steve;