(* othello.ml *)
(* オセロ対戦AI *)
(* created: 2016/06/14 *)

(*オセロ型を定義*)  
type othello = Black | White | None_ ;;


(* board が 8*8 のサイズになっているか確認 *)
let rec is_othello board =
  if (Array.length board = 8 && Array.length board.(0) = 8)
  then true else false ;;


(* 組の計算 *)
let left_inc bw =
  match bw with
  | (b, w) -> (b + 1, w) ;;

let right_inc bw =
  match bw with
  | (b, w) -> (b , w + 1) ;;

let add_tuple t1 t2 =
  match t1 with
  |(t1l, t1r) ->
      match t2 with
      |(t2l, t2r) -> (t1l + t2l, t1r+ t2r) ;;


(* board を list にしたり、array に戻したりする *)
let rec to_array board =
  let b = Array.of_list board in
  Array.map Array.of_list b ;;

let rec  to_list board =
  let b = Array.to_list board in
  List.map Array.to_list b ;;


(* (Black, White) の形式で個数を数える *)
let rec sub_counter row =
  match row with
  | [] -> (0, 0)
  | Black :: r -> left_inc (sub_counter r)
  | White :: r -> right_inc (sub_counter r)
  | None_ :: r -> sub_counter r ;;

let rec counter board =
  let b = to_list board in
  match b with
  | [] -> (0, 0)
  | b1 :: b2 ->
      let bw = sub_counter b1 in
      let b2' = to_array b2 in
      add_tuple bw (counter b2') ;;

let rec countup board =
  if (is_othello board) then counter board
  else failwith "This is not a board." ;;


(* board の初期状態はこれ *)
let board = [|[|None_; None_; None_; None_; None_; None_; None_; None_|];
	      [|None_; None_; None_; None_; None_; None_; None_; None_|];
	      [|None_; None_; None_; None_; None_; None_; None_; None_|];
	      [|None_; None_; None_; White; Black; None_; None_; None_|];
	      [|None_; None_; None_; Black; White; None_; None_; None_|];
	      [|None_; None_; None_; None_; None_; None_; None_; None_|];
	      [|None_; None_; None_; None_; None_; None_; None_; None_|];
	      [|None_; None_; None_; None_; None_; None_; None_; None_|];|] ;;


let _ = is_othello board ;;
(* - : bool = true *)
 
let _ = counter board ;;
(* - : int * int = (2, 2) *)


(* board 上の othello をひっくり返す *)
(* vec の方向へ、othello にあたるまでひっくり返す *)
(* board の中身を破壊してしまうので注意 *)
let rec sub_reverse othello =
  match othello with
  | Black -> White
  | White -> Black
  | None_ -> failwith"This thello is None_." ;;

let rec reverse rown linen board othello vec =
  match vec with
  | (0, 0) -> board
  | (r, l) ->
      let rn = rown + r and ln = linen + l in
      if ((rn < 1 || rn > 8) || (ln < 1 || ln > 8))
      then failwith "reverse"
      else
	if (board.(rn - 1).(ln - 1) = sub_reverse othello) then (
	  board.(rn - 1).(ln - 1)
	  <- othello;
	  let b = reverse rn ln board othello vec in b)
	else board ;;


(* bord 上で othello を置ける位置を探す *)
let rec sub_can_put rown linen board othello rvec lvec =
  let rn = rown + rvec and ln = linen + lvec in
  if ((rn < 1 || rn > 8) || (ln < 1 || ln > 8)) then
    false
  else if (board.(rn - 1).(ln - 1) = None_) then
    false
  else if (board.(rn - 1).(ln - 1) = othello) then
    true
  else sub_can_put rn ln board othello rvec lvec;;

sub_can_put 1 1 board Black 1 1 ;;
(* - : bool = false *)
sub_can_put 3 4 board Black 1 0 ;;
(* - : bool = true *)

let rec sub_can_put2 rown linen board othello rvec lvec =
  let rn = rown + rvec and ln = linen + lvec in
  if ((rn < 1 || rn > 8) || (ln < 1 || ln > 8)) then
    false
  else if (board.(rn - 1).(ln - 1) = othello) then
    false
  else if (board.(rn - 1).(ln - 1) = None_) then
    false
  else sub_can_put rn ln board othello rvec lvec;;

sub_can_put2 6 6 board White (-1) (-1) ;;
(* - : bool = false *)
sub_can_put2 4 6 board White 0 (-1) ;;
(* - : bool = true *)

let rec can_put rown linen board othello =
  let veclist = ref [] in
  if ((rown < 1 || rown > 8) || (linen < 1 || linen > 8))
  then veclist
  else if (board.(rown - 1).(linen - 1) != None_)
  then veclist
  else if (is_othello board = false)
  then veclist
  else
    (* 各方向に対してひっくり返せる othello があるかを調べる *)
    if (sub_can_put2 rown linen board othello 0 (-1))
    then veclist := (0, -1) :: !veclist
    else if (sub_can_put2 rown linen board othello 1 (-1))
    then veclist := (1, -1) :: !veclist
    else if (sub_can_put2 rown linen board othello 1 0)
    then veclist := (1, 0) :: !veclist
    else if (sub_can_put2 rown linen board othello 1 1)
    then veclist := (1, 1) :: !veclist
    else if (sub_can_put2 rown linen board othello 0 1)
    then veclist := (0, 1) :: !veclist
    else if (sub_can_put2 rown linen board othello (-1) 1)
    then veclist := (-1, 1) :: !veclist
    else if (sub_can_put2 rown linen board othello (-1) 0)
    then veclist := (-1, 0) :: !veclist
    else if (sub_can_put2 rown linen board othello (-1) (-1))
    then veclist := (-1, -1) :: !veclist;
  veclist ;;
(* this function is unfinished *)


(* othello を board 上に置く *)
(* これも board の中身を破壊している *)
let rec put rown linen board othello =
  if (can_put rown linen board othello) then (
    board.(rown - 1).(linen - 1) <- othello;

    ...
      
    reverse rown linen board othello (...)
   )
  else failwith "you can not put there." ;;
(* this function is unfinished *)
