-- 8.3 use D FF and combinational circuit to design a JK. Dervie VHDL code and draw diagram

library ieee;
use ieee.std_logic_1164.all;
entity jkff is
	port(
		clk: in std_logic;
		j: in std_logic;
		k: in std_logic
		q: out std_logic
	);
end jkff;

architecture arch of jkff is
	signal q_next : std_logic;
	signal temp : std_logic_vector;
	signal q_reg : std_logic;
begin 
	
	process(clk)
	begin
		if (clk'event and clk='1') then
			q_reg <= q_next;
		end if;
	end process;
	
	-- next state logic
	temp <= j & k;
	q_next <= '0' when temp = "01" else
		'1' when temp = "10" else
		q when temp = "00" else
		not q;
	
	-- out logic
	q <= q_reg;
	
end arch;



--2 Derive VHDL code for a T FF that is negatively triggered and uses a synchronus reset 

library ieee;
use ieee.std_logic_1164.all;
entity tff is
	port(
		clk: in std_logic;
		reset: in std_logic;
		t: in std_logic
		q: out std_logic
	);
end tff;

architecture arch of tff is
	signal q_reg : std_logic;
	signal q_next: std_logic_vector;
begin 
	process(clk) -- not sensitive to reset b/c of synchronus reset
	begin
		if  (reset = '1' and clk'event and clk = '0') then
			q_reg <= '0';
		elsif (clk'event and clk='0') then
			q_reg <= q_next;
		end if;
	end process;

-- next state logic
q_next <= q_reg when t = '0' else
	not(q_reg);

--out logic
q <= q_reg;

end arch;



--8.5 Expand universal shifter to include rotate right and left operations, ctrl must be extended to 3 bits
library ieee;
use ieee.std_logic_1164.all;
entity shift_register is
	port(
	clk, reset: in std_logic;
	ctrl: in std_logic_vector (2 downto 0); --control now 3 bits
	d: in std_logic_vector (3 downto 0);
	q: out std_logic_vector(3 downto o)
	);
end shift_register;

architecture two_seg_arch of shift_register is
	signal r_reg: std_logic_vector(3 downto 0);
	signal r_next: std_logic_vector(3 downto 0);
begin
 --register
 process(clk, reset)
 begin
	if (reset = '1') then
		r_reg <= (others => '0');
	elsif (clk'event and clk='1') then
		r_reg <= r_next;
	end if;
end process;
--next state logic
with ctrl select
	r_next <=
		r_reg when "000", --pause
		r_reg(2 downto 0) & d(0) when "001", --shift left
		d(3) & r_reg(3 downto 1) when "010", --shift right
		r_reg(2 downto 0) & r_reg(3) when "100", --rotate left
		r_reg(0) & r_reg(3 downto 1) when "101", --rotate right
		d when others;
	
--output logic
q <= r_reg;
end two_seg_arch




--8.7 4bit counter and draw conceptual diagram

library ieee;
use ieee.std_logic_1164.all;

entity arbi_seq_counter4 is
	port(
	clk, reset: in std_logic;
	q: out std_logic_vector(3 downto 0)
	);
end arbi_seq_counter4;

architecture two_seg_arch of arbi_seq_counter4 is
	signal r_reg: std_logic_vector(3 downto 0);
	signal r_next: std_logic_vector(3 downto 0);
begin
	--register
	process(clk, reset) -- added a asynchronous reset to our circuit
	begin
		if (reset = '1') then
			r_reg <= (others => '0');
		elsif (clk'event and clk='1') then
			r_reg <= r_next;
		end if;
	end process;
	--next-state logic
	with r_reg select
	r_next <= "0100" when "0011", -- 4 when 3
			"0101" when "0100", -- 5 when 4	
			"0110" when "0101", -- 6 when 5
			"0111" when "0110", -- 7 when 6
			"1000" when "0111", -- 8 when 7
			"1001" when "1000", -- 9 when 8
			"1010" when "1001", -- 10 when 9
			"1011" when "1010", -- 11 when 10
			"1100" when "1011", -- 12 when 11
			"0011" when others;
			
	--output logic
	q <= r_reg;
end two_seg_arch;




-- 8.8 redesign arbitrary counter, used mod5 counter 

library ieee;
use ieee.std_logic_1164.all;

entity arbi_mod5 is
	port(
	clk, reset: in std_logic;
	q: out std_logic_vector(2 downto 0)
	);
end arbi_mod5;

architecture arch of arbi_mod5 is
	signal r_reg: unsigned(2 downto 0);
	signal r_next: unsigned(2 downto 0);
begin
	--register
	process(clk, reset)
	begin
		if (reset = '1') then
			r_reg <= (others => '0');
		elsif (clk'event and clk='1') then
			r_reg <= r_next;
		end if;
	end process;
	--next-state logic
	
	-- mod 5
	r_next <= (others =>'0') when r_reg = 4 else
				r_reg + 1;

	-- decoding circuitry output logic
	with r_reg select
	q <= "000" when "000", -- mod 5 is 0
			"011" when "001", -- mod 5 is 1 
			"110" when "010", -- mod 5 is 2 
			"101" when "011", -- mod 5 is 3 
			"111" when others; -- mod 5 is 4 
end arch;




-- 8.10 assume 1 Mhz clk, generates 1 hz output pulse with 50% duty cycle. 
library ieee;
use ieee.std_logic_1164.all;

entity pulse_count is
	port(
	clk, reset: in std_logic;
	q: out std_logic_vector(31 downto 0)
	);
end pulse_count;

architecture arch of pulse_count is
	signal r_reg: unsigned(31 downto 0);
	signal r_next: unsigned(31 downto 0);
	signal q_reg: std_logic;
	signal q_next: std_logic;
begin
	--register
	process(clk, reset)
	begin
		if (reset = '1') then
			r_reg <= (others => '0');
			q_reg <= '0';
		elsif (clk'event and clk='1') then
			r_reg <= r_next;
			q_reg <= q_next;
		end if;
	end process;
	--next-state logic
	
	r_next <= (others =>'0') when r_reg = 5E5 else
				r_reg + 1;
	q_next <= not q_reg when r_reg 5E5 else 
				r_reg;

	--output logic
	q <= q_reg;
end arch;


-- Ch. 8.6 Problems
--Tsetup = 3ns
--Thold = 1ns
--Tcq = 5ns

1. What is	the	minimum	clock	period,	Tc,	and	the	maximum	frequency,	fmax,	of	a	
	sequential	system	that	has	next	state	logic	requiring	15	ns	of	propagation	delay	
	(i.e.,	Tnext(max) =	15	ns)?

	min_clock = tcq + tnext(max) + tsetup = 5ns + 15ns + 3ns = 23 ns
	f_max = 1/min_clock = 43,478,261 Hz = 43.48 Mhz
	
2. Why	is	Tnext(max) used	in	the	equation	for	minimum	clock	period	instead	of	
    Tnext(min)?
	
	Because we are looking at worse case scenarios for propagation delay. If it was over the tnext(min)
	it could still be enough time for the lcok and violate our timing requirements. 
	
3. In the	timing	diagram	of	Figure	8.15, why	must	the	event	marked	as	t3 occur	
	before	the	event	marked	as	t4?
	
	The event marked as t3 refers to when the state_next register has stablized, meaning that the tcq has 
	already been statisfied along with tnext(max) t4 refers to the setup time before the next clock cycle
	comes. It must occure before so that the value can be stable before getting assigned. 
	
4. 	What	is	the	maximum	clock	frequency	available	using	this	flip-flop (i.e.,	when	
	tnext(max) =	0)?
	
		min_clock = tcq + tnext(max) + tsetup = 5ns + 0ns + 3ns = 8 ns
	f_max = 1/min_clock = 125 Mhz
	
	
	

	

	


